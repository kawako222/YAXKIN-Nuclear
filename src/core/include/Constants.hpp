#ifndef YAXKIN_CONSTANTS_HPP
#define YAXKIN_CONSTANTS_HPP

#include <vector>

namespace Yaxkin {
    struct NuclearConstants {
        static constexpr double PROMPT_GEN_TIME = 1e-4; 
        static constexpr double TOTAL_BETA = 0.0065;
        static constexpr double WATER_DENSITY = 750.0;     // kg/m3 (a alta presión)
        static constexpr double SPECIFIC_HEAT = 5000.0;    // J/(kg*K)
        static constexpr double FUEL_MASS = 20000.0;       // kg (un SMR pequeño)
        static constexpr double FEEDBACK_COEFF = -0.0001;  // dk/dT (Reactividad por cada grado de calor)
        static constexpr double ALPHA_FUEL = -1.2e-5;    // Coeficiente Doppler (Rápido)
        static constexpr double ALPHA_MOD  = -0.00015;   // Coeficiente Moderador (Lento)
        static constexpr double HEAT_TRANSFER_COEFF = 5.0e6; // W/K (Transferencia fuel -> agua)
        static constexpr double MASS_FLOW_RATE = 1500.0; // kg/s (Flujo de la bomba)
        static constexpr double INLET_TEMP = 275.0;      // °C (Agua regresando del generador)

        // --- DINÁMICA DE VENENOS NEUTRÓNICOS (Xenón-135) ---
        
        // Fracciones de rendimiento (Qué porcentaje de las fisiones crea estos elementos)
        static constexpr double YIELD_I = 0.0639;  // Rendimiento de Yodo-135 (6.39%)
        static constexpr double YIELD_XE = 0.00237; // Rendimiento directo de Xenón-135 (0.237%)

        // Constantes de decaimiento radiactivo (en s^-1)
        static constexpr double LAMBDA_I = 2.87e-5;  // Vida media ~ 6.7 horas
        static constexpr double LAMBDA_XE = 2.09e-5; // Vida media ~ 9.2 horas

        // Sección eficaz microscópica de absorción del Xenón (cm^2)
        // Esto define qué tan "hambriento" es el Xenón por los neutrones
        static constexpr double SIGMA_A_XE = 2.65e-18; 
        
        // Constante de conversión de flujo a potencia (Flujo nominal típico de un SMR)
        static constexpr double NOMINAL_FLUX = 3.0e13; // neutrones / (cm^2 * s)

        inline static const std::vector<double> BETA_I = {
            0.000215, 0.001424, 0.001274, 0.002568, 0.000748, 0.000273
        };

        inline static const std::vector<double> LAMBDA_I = {
            0.0124, 0.0305, 0.111, 0.301, 1.14, 3.01
        };
    };
} 

#endif