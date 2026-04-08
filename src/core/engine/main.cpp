#include <iostream>
#include <iomanip>
#include "ReactorCore.hpp"

int main() {


    ///////////////////////MAIN FUNCTION////////////7
    
    Yaxkin::ReactorCore myReactor;
    double dt = 0.1; // Paso de tiempo de 100ms
    double total_time = 10.0;
    
    std::cout << "---  SIMULACIÓN YAXKIN-1  ---" << std::endl;
    std::cout << "Tiempo(s) | Potencia | Reactividad" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    for (double t = 0; t <= total_time; t += dt) {
        double rho_input = (t > 1.0 && t < 2.0) ? 0.001 : 0.0;
        
        myReactor.update(dt, rho_input);

        std::cout << std::fixed << std::setprecision(2) 
                  << t << "s      | " 
                  << std::setprecision(4) << myReactor.getPower() << "    | " 
                  << myReactor.getReactivity() << std::endl;
    }

    return 0;
    
}