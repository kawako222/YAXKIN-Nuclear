#include "ReactorCore.hpp"
#include <iostream>

namespace Yaxkin {

    ReactorCore::ReactorCore() : current_reactivity(0.0) {
        base_temperature = 315.0; // Basado en el agua
        t_coolant = 315.0;        // El agua debe estar a 315C para sacar 300MW
        t_fuel = 375.0;           // El fuel debe estar a 375C para pasar calor al agua

        // No pasaron la prueba de la derivada nula
        /*
        base_temperature = 280.0;
        t_fuel = 280.0;     
        t_coolant = 280.0;  
        */


        // El vector crece a 11 dimensiones
        state = Eigen::VectorXd::Zero(11);
        state(0) = 1.0; 

        // Inicializamos precursores
        for (int i = 0; i < 6; ++i) {
            state(i + 1) = (NuclearConstants::BETA_I[i] * state(0)) / 
                           (NuclearConstants::LAMBDA_I[i] * NuclearConstants::PROMPT_GEN_TIME);
        }

        // Constantes macroscópicas asumidas para un PWR típico
        double sigma_f = 0.05; 
        double phi_nominal = NuclearConstants::NOMINAL_FLUX;

        // Inicializamos Yodo-135 en equilibrio (Nombres corregidos)
        state(7) = (NuclearConstants::YIELD_IODINE * sigma_f * phi_nominal) / NuclearConstants::LAMBDA_IODINE;

        // Inicializamos Xenón-135 en equilibrio (Nombres corregidos)
        state(8) = (NuclearConstants::YIELD_XE * sigma_f * phi_nominal + NuclearConstants::LAMBDA_IODINE * state(7)) / 
                   (NuclearConstants::LAMBDA_XE + NuclearConstants::SIGMA_A_XE * phi_nominal);

        // Inicializamos Prometio en equilibrio
        state(9) = (NuclearConstants::YIELD_PM * sigma_f * phi_nominal) / NuclearConstants::LAMBDA_PM;

        // Inicializamos Samario en equilibrio
        state(10) = (NuclearConstants::YIELD_PM * sigma_f * phi_nominal) / (NuclearConstants::SIGMA_A_SM * phi_nominal);
    }

    Eigen::VectorXd ReactorCore::computeDerivatives(const Eigen::VectorXd& current_state, double effective_rho) {
        Eigen::VectorXd dState = Eigen::VectorXd::Zero(11);
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

        // 3. Derivada del Yodo-135 (dI/dt) (Nombres corregidos)
        dState(7) = NuclearConstants::YIELD_IODINE * sigma_f * phi_nominal * current_power - 
                    NuclearConstants::LAMBDA_IODINE * current_state(7);

        // 4. Derivada del Xenón-135 (dXe/dt) (Nombres corregidos)
        dState(8) = NuclearConstants::YIELD_XE * sigma_f * phi_nominal * current_power 
                    + NuclearConstants::LAMBDA_IODINE * current_state(7) 
                    - NuclearConstants::LAMBDA_XE * current_state(8) 
                    - NuclearConstants::SIGMA_A_XE * current_state(8) * phi_nominal * current_power;
        
        // 5. Derivada del Prometio-149 (dPm/dt)
        dState(9) = NuclearConstants::YIELD_PM * sigma_f * phi_nominal * current_power - 
                    NuclearConstants::LAMBDA_PM * current_state(9);

        // 6. Derivada del Samario-149 (dSm/dt)
        // Se crea del Prometio y se destruye por absorción de neutrones (no decae solo)
        dState(10) = NuclearConstants::LAMBDA_PM * current_state(9) - 
                     NuclearConstants::SIGMA_A_SM * current_state(10) * phi_nominal * current_power;

        
        return dState;
    }

    void ReactorCore::update(double dt, double reactivity_input) {
        current_reactivity = reactivity_input;
        
        int sub_steps = 10; 
        double h = dt / sub_steps;

        for (int i = 0; i < sub_steps; ++i) {
            
            
            // --- 1. Calcular Feedback de Reactividad Combinado ---
            double feedback_fuel = (t_fuel - 375.0) * NuclearConstants::ALPHA_FUEL;
            double feedback_coolant = (t_coolant - 315.0) * NuclearConstants::ALPHA_MOD;
            
            // --- Aplicar el freno del Xenón ---
            double sigma_a_total = 0.1; 
            double xenon_rho = - (state(8) * NuclearConstants::SIGMA_A_XE) / sigma_a_total;

            static double initial_xenon_rho = xenon_rho; 
            double net_xenon_rho = xenon_rho - initial_xenon_rho;

            // --- freno del Samario  ---
            double samarium_rho = - (state(10) * NuclearConstants::SIGMA_A_SM) / sigma_a_total;
            static double initial_samarium_rho = samarium_rho;
            double net_samarium_rho = samarium_rho - initial_samarium_rho;
            
            // La reactividad efectiva tiene en cuenta el calor y el veneno
            double effective_rho = current_reactivity + feedback_fuel + feedback_coolant + net_xenon_rho;
            

            // --- 2. Integración Nuclear (RK4) ---
            Eigen::VectorXd k1 = computeDerivatives(state, effective_rho);
            Eigen::VectorXd k2 = computeDerivatives(state + 0.5 * h * k1, effective_rho);
            Eigen::VectorXd k3 = computeDerivatives(state + 0.5 * h * k2, effective_rho);
            Eigen::VectorXd k4 = computeDerivatives(state + h * k3, effective_rho);

            state += (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);

            if (state(0) < 0.0) state(0) = 0.0; 

            // --- 3. Integración Termohidráulica (Lazo de Transferencia) ---
            double power_watts = state(0) * 300e6; 
            double heat_transfer = NuclearConstants::HEAT_TRANSFER_COEFF * (t_fuel - t_coolant);
            double heat_removed = NuclearConstants::MASS_FLOW_RATE * NuclearConstants::SPECIFIC_HEAT * (t_coolant - NuclearConstants::INLET_TEMP);

            double dt_fuel = (power_watts - heat_transfer) / (NuclearConstants::FUEL_MASS * NuclearConstants::SPECIFIC_HEAT);
            double dt_coolant = (heat_transfer - heat_removed) / (15000.0 * NuclearConstants::SPECIFIC_HEAT); 

            t_fuel += dt_fuel * h;
            t_coolant += dt_coolant * h;
        }
    }

} // namespace Yaxkin