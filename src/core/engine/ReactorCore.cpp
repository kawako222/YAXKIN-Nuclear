#include "ReactorCore.hpp"
#include <iostream>

namespace Yaxkin {

    ReactorCore::ReactorCore() : current_reactivity(0.0) {
        base_temperature = 315.0;
        t_coolant        = 315.0;
        t_fuel           = 375.0;

        state = Eigen::VectorXd::Zero(11);
        state(0) = 1.0; // Potencia normalizada al 100%

        // --- Equilibrio de Precursores ---
        for (int i = 0; i < 6; ++i) {
            state(i + 1) = (NuclearConstants::BETA_I[i] * state(0)) /
                           (NuclearConstants::LAMBDA_I[i] * NuclearConstants::PROMPT_GEN_TIME);
        }

        // Flujo y sección eficaz en condiciones nominales (potencia = 1.0)
        const double phi_0 = NuclearConstants::NOMINAL_FLUX * state(0); // = NOMINAL_FLUX × 1.0

        // --- Equilibrio de Yodo-135 ---
        state(7) = (NuclearConstants::YIELD_IODINE * NuclearConstants::SIGMA_F * phi_0) /
                   NuclearConstants::LAMBDA_IODINE;

        // --- Equilibrio de Xenón-135 ---
        state(8) = (NuclearConstants::YIELD_XE * NuclearConstants::SIGMA_F * phi_0 +
                    NuclearConstants::LAMBDA_IODINE * state(7)) /
                   (NuclearConstants::LAMBDA_XE + NuclearConstants::SIGMA_A_XE * phi_0);

        // --- Equilibrio de Prometio-149 ---
        state(9) = (NuclearConstants::YIELD_PM * NuclearConstants::SIGMA_F * phi_0) /
                   NuclearConstants::LAMBDA_PM;

        // --- Equilibrio de Samario-149 ---
        state(10) = (NuclearConstants::YIELD_PM * NuclearConstants::SIGMA_F * phi_0) /
                    (NuclearConstants::SIGMA_A_SM * phi_0);

        // FIX: Inicializamos los rho base de venenos como miembros de instancia.
        // Antes eran `static` locales en update(), lo que significaba que la
        // primera instancia contaminaba todas las instancias posteriores.
        // Ahora cada ReactorCore tiene su propio punto de referencia correcto.
        initial_xenon_rho     = -(state(8)  * NuclearConstants::SIGMA_A_XE) /
                                  NuclearConstants::SIGMA_A_TOTAL;
        initial_samarium_rho  = -(state(10) * NuclearConstants::SIGMA_A_SM) /
                                  NuclearConstants::SIGMA_A_TOTAL;
    }

    // ---------------------------------------------------------------------------
    // computeDerivatives
    // FIX (flujo neutrónico): el flujo phi ahora escala con la potencia actual.
    // Antes se usaba siempre NOMINAL_FLUX, lo que sobreestimaba la destrucción
    // de Xe-135 a baja potencia y subestimaba el efecto del "Xenon pit" post-scram.
    // La corrección es: phi = NOMINAL_FLUX × current_power (normalizado a 1.0 = 300 MW)
    // ---------------------------------------------------------------------------
    Eigen::VectorXd ReactorCore::computeDerivatives(const Eigen::VectorXd& current_state,
                                                    double effective_rho) {
        Eigen::VectorXd dState = Eigen::VectorXd::Zero(11);
        const double current_power = current_state(0);

        // FIX: flujo real proporcional a la potencia del reactor en este instante
        const double phi = NuclearConstants::NOMINAL_FLUX * current_power;

        // --- 1. dn/dt  (ecuación de cinética puntual) ---
        double sum_lambda_C = 0.0;
        for (int i = 0; i < 6; ++i) {
            sum_lambda_C += NuclearConstants::LAMBDA_I[i] * current_state(i + 1);
        }
        dState(0) = ((effective_rho - NuclearConstants::TOTAL_BETA) /
                     NuclearConstants::PROMPT_GEN_TIME) * current_power + sum_lambda_C;

        // --- 2. dCi/dt  (precursores) ---
        for (int i = 0; i < 6; ++i) {
            dState(i + 1) = (NuclearConstants::BETA_I[i] / NuclearConstants::PROMPT_GEN_TIME) *
                            current_power -
                            NuclearConstants::LAMBDA_I[i] * current_state(i + 1);
        }

        // --- 3. dI/dt  (Yodo-135) ---
        dState(7) = NuclearConstants::YIELD_IODINE * NuclearConstants::SIGMA_F * phi
                  - NuclearConstants::LAMBDA_IODINE * current_state(7);

        // --- 4. dXe/dt  (Xenón-135) ---
        // Término de destrucción: SIGMA_A_XE × Xe × phi  (escala con potencia — FIX clave)
        dState(8) = NuclearConstants::YIELD_XE    * NuclearConstants::SIGMA_F * phi
                  + NuclearConstants::LAMBDA_IODINE * current_state(7)
                  - NuclearConstants::LAMBDA_XE     * current_state(8)
                  - NuclearConstants::SIGMA_A_XE    * current_state(8) * phi;

        // --- 5. dPm/dt  (Prometio-149) ---
        dState(9) = NuclearConstants::YIELD_PM * NuclearConstants::SIGMA_F * phi
                  - NuclearConstants::LAMBDA_PM * current_state(9);

        // --- 6. dSm/dt  (Samario-149) ---
        dState(10) = NuclearConstants::LAMBDA_PM  * current_state(9)
                   - NuclearConstants::SIGMA_A_SM * current_state(10) * phi;

        return dState;
    }

    void ReactorCore::update(double dt, double reactivity_input) {
        current_reactivity = reactivity_input;

        const int sub_steps = 10;
        const double h = dt / sub_steps;

        for (int i = 0; i < sub_steps; ++i) {

            // --- Feedback de temperatura ---
            double feedback_fuel    = (t_fuel    - 375.0) * NuclearConstants::ALPHA_FUEL;
            double feedback_coolant = (t_coolant - 315.0) * NuclearConstants::ALPHA_MOD;

            // --- Reactividad del Xenón (delta respecto al equilibrio inicial) ---
            double xenon_rho     = -(state(8)  * NuclearConstants::SIGMA_A_XE) /
                                     NuclearConstants::SIGMA_A_TOTAL;
            double net_xenon_rho = xenon_rho - initial_xenon_rho;

            // --- Reactividad del Samario (delta respecto al equilibrio inicial) ---
            double samarium_rho     = -(state(10) * NuclearConstants::SIGMA_A_SM) /
                                        NuclearConstants::SIGMA_A_TOTAL;
            double net_samarium_rho = samarium_rho - initial_samarium_rho;

            double effective_rho = current_reactivity
                                 + feedback_fuel
                                 + feedback_coolant
                                 + net_xenon_rho
                                 + net_samarium_rho; // Samario también incluido ahora

            // --- Integración nuclear RK4 ---
            Eigen::VectorXd k1 = computeDerivatives(state,                   effective_rho);
            Eigen::VectorXd k2 = computeDerivatives(state + 0.5 * h * k1,   effective_rho);
            Eigen::VectorXd k3 = computeDerivatives(state + 0.5 * h * k2,   effective_rho);
            Eigen::VectorXd k4 = computeDerivatives(state + h        * k3,   effective_rho);

            state += (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
            if (state(0) < 0.0) state(0) = 0.0;

            // --- Integración termohidráulica ---
            const double power_watts = state(0) * 300e6;
            const double heat_transfer = NuclearConstants::HEAT_TRANSFER_COEFF *
                                         (t_fuel - t_coolant);
            const double heat_removed  = NuclearConstants::MASS_FLOW_RATE *
                                         NuclearConstants::SPECIFIC_HEAT *
                                         (t_coolant - NuclearConstants::INLET_TEMP);

            const double dt_fuel    = (power_watts - heat_transfer) /
                                      (NuclearConstants::FUEL_MASS * NuclearConstants::SPECIFIC_HEAT);
            const double dt_coolant = (heat_transfer - heat_removed) /
                                      (15000.0 * NuclearConstants::SPECIFIC_HEAT);

            t_fuel    += dt_fuel    * h;
            t_coolant += dt_coolant * h;
        }
    }

    // ---------------------------------------------------------------------------
    // updateIsothermal — cinética puntual pura
    // Congela t_fuel, t_coolant y venenos: solo integra la potencia y precursores.
    // La reactividad efectiva es exactamente la reactividad de entrada, sin
    // ninguna retroalimentación. Exclusivo para V&V 4 (medición de período T).
    // ---------------------------------------------------------------------------
    void ReactorCore::updateIsothermal(double dt, double reactivity_input) {
        current_reactivity = reactivity_input;

        const int    sub_steps = 10;
        const double h         = dt / sub_steps;

        for (int i = 0; i < sub_steps; ++i) {
            // Sin feedback: la reactividad efectiva es solo la entrada
            Eigen::VectorXd k1 = computeDerivatives(state,                 reactivity_input);
            Eigen::VectorXd k2 = computeDerivatives(state + 0.5 * h * k1, reactivity_input);
            Eigen::VectorXd k3 = computeDerivatives(state + 0.5 * h * k2, reactivity_input);
            Eigen::VectorXd k4 = computeDerivatives(state + h        * k3, reactivity_input);

            state += (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
            if (state(0) < 0.0) state(0) = 0.0;
            // t_fuel y t_coolant no se tocan — reactor isotérmico
        }
    }

} // namespace Yaxkin