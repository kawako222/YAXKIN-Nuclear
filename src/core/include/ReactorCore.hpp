#ifndef YAXKIN_REACTOR_CORE_HPP
#define YAXKIN_REACTOR_CORE_HPP

#include "Constants.hpp"
#include <Eigen/Dense>
#include <vector>

namespace Yaxkin {

    class ReactorCore {
    public:
        ReactorCore();

        // Actualiza el estado del reactor en un paso de tiempo dt
        void update(double dt, double reactivity_input);

        // Getters para ver qué está pasando
        double getTemperature() const { return temperature; }
        double getPower() const { return state(0); }
        double getReactivity() const { return current_reactivity; }

    private:
        // Vector de estado ahora es de 9 posiciones: 
        // [0] = Potencia
        // [1..6] = Precursores de neutrones
        // [7] = Concentración de Yodo-135
        // [8] = Concentración de Xenón-135
        Eigen::VectorXd state;        // Vector de estado: [0]=Potencia, [1..6]=Precursores

        double current_reactivity;   
        double t_fuel;             // Temperatura del combustible
        double t_coolant;          // Temperatura del refrigerante (agua)
        double temperature;           // Agregado para termohidráulica
        double base_temperature;      // Agregado para termohidráulica

        Eigen::VectorXd computeDerivatives(const Eigen::VectorXd& current_state, double effective_rho);        void solveKinetics(double dt, double rho);
    };

} // namespace Yaxkin

#endif