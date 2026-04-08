#include "ReactorCore.hpp"
#include <iostream>

namespace Yaxkin {

    ReactorCore::ReactorCore() : current_reactivity(0.0) {
        base_temperature = 280.0;
        t_fuel = 280.0;     
        t_coolant = 280.0;  
        
        // El vector crece a 9 dimensiones
        state = Eigen::VectorXd::Zero(9);
        state(0) = 1.0; 

        // Inicializamos precursores
        for (int i = 0; i < 6; ++i) {
            state(i + 1) = (NuclearConstants::BETA_I[i] * state(0)) / 
                           (NuclearConstants::LAMBDA_I[i] * NuclearConstants::PROMPT_GEN_TIME);
        }

        // Constantes macroscópicas asumidas para un PWR típico
        double sigma_f = 0.05; // Sección eficaz macroscópica de fisión (cm^-1)
        double phi_nominal = NuclearConstants::NOMINAL_FLUX;

        // Inicializamos Yodo-135 en equilibrio
        state(7) = (NuclearConstants::YIELD_I * sigma_f * phi_nominal) / NuclearConstants::LAMBDA_I;

        // Inicializamos Xenón-135 en equilibrio
        state(8) = (NuclearConstants::YIELD_XE * sigma_f * phi_nominal + NuclearConstants::LAMBDA_I * state(7)) / 
                   (NuclearConstants::LAMBDA_XE + NuclearConstants::SIGMA_A_XE * phi_nominal);
    }

    Eigen::VectorXd ReactorCore::computeDerivatives(const Eigen::VectorXd& current_state, double effective_rho) {
        Eigen::VectorXd dState = Eigen::VectorXd::Zero(9);
        double current_power = current_state(0);
        
        // 1. Derivada de la Potencia (dn/dt)
        double sum_lambda_C = 0.0;
        for (int i = 0; i < 6; ++i) {
            sum_lambda_C += NuclearConstants::LAMBDA_I[i] * current_state(i + 1);
        }

        dState(0) = ((effective_rho - NuclearConstants::TOTAL_BETA) / NuclearConstants::PROMPT_GEN_TIME) * current_power + sum_lambda_C;
        
        // 2. Derivadas de los Precursores (dCi/dt)
        for (int i = 0; i < 6; ++i) {
            dState(i + 1) = (NuclearConstants::BETA_I[i] / NuclearConstants::PROMPT_GEN_TIME) * current_power - 
                            NuclearConstants::LAMBDA_I[i] * current_state(i + 1);
        }

        // Constantes para el cálculo de venenos
        double sigma_f = 0.05; 
        double phi_nominal = NuclearConstants::NOMINAL_FLUX;

        // 3. Derivada del Yodo-135 (dI/dt)
        dState(7) = NuclearConstants::YIELD_I * sigma_f * phi_nominal * current_power - 
                    NuclearConstants::LAMBDA_I * current_state(7);

        // 4. Derivada del Xenón-135 (dXe/dt)
        // Se crea por fisión directa + decaimiento del Yodo. Se destruye por decaimiento natural + absorción de neutrones.
        dState(8) = NuclearConstants::YIELD_XE * sigma_f * phi_nominal * current_power 
                    + NuclearConstants::LAMBDA_I * current_state(7) 
                    - NuclearConstants::LAMBDA_XE * current_state(8) 
                    - NuclearConstants::SIGMA_A_XE * current_state(8) * phi_nominal * current_power;

        return dState;
    }

    void ReactorCore::update(double dt, double reactivity_input) {
        current_reactivity = reactivity_input;
        
        int sub_steps = 10; 
        double h = dt / sub_steps;

        for (int i = 0; i < sub_steps; ++i) {
            
            // --- 1. Calcular Feedback de Reactividad Combinado ---
            // Doppler (Combustible) + Moderador (Agua/Refrigerante)
            double feedback_fuel = (t_fuel - base_temperature) * NuclearConstants::ALPHA_FUEL;
            double feedback_coolant = (t_coolant - base_temperature) * NuclearConstants::ALPHA_MOD;
            double effective_rho = current_reactivity + feedback_fuel + feedback_coolant;

            // --- 2. Integración Nuclear (RK4) ---
            // Pasamos la effective_rho para que los neutrones "sientan" el calor
            Eigen::VectorXd k1 = computeDerivatives(state, effective_rho);
            Eigen::VectorXd k2 = computeDerivatives(state + 0.5 * h * k1, effective_rho);
            Eigen::VectorXd k3 = computeDerivatives(state + 0.5 * h * k2, effective_rho);
            Eigen::VectorXd k4 = computeDerivatives(state + h * k3, effective_rho);

            state += (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);

            if (state(0) < 0.0) state(0) = 0.0; // Failsafe

            // --- 3. Integración Termohidráulica (Lazo de Transferencia) ---
            
            // A. Calor generado por la fisión en el combustible
            double power_watts = state(0) * 300e6; 
            
            // B. Calor que pasa del Combustible al Refrigerante (Transferencia de calor)
            double heat_transfer = NuclearConstants::HEAT_TRANSFER_COEFF * (t_fuel - t_coolant);
            
            // C. Calor extraído por el flujo de agua (Circulación)
            // El agua entra a INLET_TEMP y se calienta hasta t_coolant
            double heat_removed = NuclearConstants::MASS_FLOW_RATE * NuclearConstants::SPECIFIC_HEAT * (t_coolant - NuclearConstants::INLET_TEMP);

            // D. Ecuaciones diferenciales de temperatura
            double dt_fuel = (power_watts - heat_transfer) / (NuclearConstants::FUEL_MASS * NuclearConstants::SPECIFIC_HEAT);
            double dt_coolant = (heat_transfer - heat_removed) / (15000.0 * NuclearConstants::SPECIFIC_HEAT); 

            // E. Actualización de temperaturas (Euler)
            t_fuel += dt_fuel * h;
            t_coolant += dt_coolant * h;
        }
    }

} // namespace Yaxkin