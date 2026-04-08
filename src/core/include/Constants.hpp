#ifndef YAXKIN_CONSTANTS_HPP
#define YAXKIN_CONSTANTS_HPP

namespace Yaxkin {

    struct NuclearConstants {
        // --- CINÉTICA DE PUNTO (Precursores) ---
        static constexpr double PROMPT_GEN_TIME = 1.0e-4; 
        static constexpr double TOTAL_BETA = 0.00649;
        
        static constexpr double BETA_I[6] = {0.00021, 0.00142, 0.00127, 0.00257, 0.00075, 0.00027};
        static constexpr double LAMBDA_I[6] = {0.0124, 0.0305, 0.111, 0.301, 1.14, 3.01};

        // --- TERMOHIDRÁULICA Y CALOR ---
        static constexpr double WATER_DENSITY = 750.0;     
        static constexpr double SPECIFIC_HEAT = 5000.0;    
        static constexpr double FUEL_MASS = 20000.0;       
        static constexpr double FEEDBACK_COEFF = -0.0001;  
        static constexpr double ALPHA_FUEL = -1.2e-5;    
        static constexpr double ALPHA_MOD  = -0.00015;   
        static constexpr double HEAT_TRANSFER_COEFF = 5.0e6; 
        static constexpr double MASS_FLOW_RATE = 1500.0; 
        static constexpr double INLET_TEMP = 275.0;      

        // --- DINÁMICA DE VENENOS NEUTRÓNICOS (Xenón y Yodo) ---
        static constexpr double YIELD_IODINE = 0.0639;  
        static constexpr double YIELD_XE = 0.00237; 
        static constexpr double LAMBDA_IODINE = 2.87e-5;  
        static constexpr double LAMBDA_XE = 2.09e-5; 
        static constexpr double SIGMA_A_XE = 2.65e-18; 

        // --- DINÁMICA DEL SAMARIO-149 ---
        static constexpr double YIELD_PM = 0.0113;       // Rendimiento de Prometio-149 (1.13%)
        static constexpr double LAMBDA_PM = 3.56e-6;     // Vida media ~ 53 horas
        static constexpr double SIGMA_A_SM = 4.1e-19;    // Sección eficaz de absorción del Samario

        static constexpr double NOMINAL_FLUX = 3.0e13;
    };

} // namespace Yaxkin

#endif