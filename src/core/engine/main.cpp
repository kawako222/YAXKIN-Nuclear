#include <iostream>
#include <iomanip>
#include "ReactorCore.hpp"

int main() {
    std::cout << "--- V&V ETAPA 1: EQUILIBRIO DE PRECURSORES ---\n";
    
    // Instanciamos el reactor (esto llama al constructor que queremos validar)
    Yaxkin::ReactorCore reactor;

    // Imprimimos los valores generados por el código
    std::cout << std::fixed << std::setprecision(4);
    for (int i = 0; i < 6; ++i) {
        std::cout << "Grupo " << (i + 1) << " | Valor en simulador: " << reactor.getPrecursor(i) << "\n";
    }

    return 0;
    /*
    Yaxkin::ReactorCore myReactor;
    double dt = 0.1; // Paso de tiempo de 100ms
    double total_time = 10.0;
    
    std::cout << "--- ☀️ SIMULACIÓN YAXKIN-1 INICIADA ---" << std::endl;
    std::cout << "Tiempo(s) | Potencia | Reactividad" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    for (double t = 0; t <= total_time; t += dt) {
        // Insertamos 0.001 de reactividad al segundo 1.0 (Reactor Supercrítico)
        double rho_input = (t > 1.0 && t < 2.0) ? 0.001 : 0.0;
        
        myReactor.update(dt, rho_input);

        std::cout << std::fixed << std::setprecision(2) 
                  << t << "s      | " 
                  << std::setprecision(4) << myReactor.getPower() << "    | " 
                  << myReactor.getReactivity() << std::endl;
    }

    return 0;
    */
}