#ifndef YAXKIN_REACTOR_CORE_HPP
#define YAXKIN_REACTOR_CORE_HPP

#include "Constants.hpp"
#include <Eigen/Dense>
#include <vector>

namespace Yaxkin {

    class ReactorCore {
    public:
        ReactorCore();

        // Estado del reactor en un paso de tiempo dt
        void update(double dt, double reactivity_input);

        double getFuelTemp() const { return t_fuel; }
        double getCoolantTemp() const { return t_coolant; }
        double getPower() const { return state(0); }
        double getReactivity() const { return current_reactivity; }
        double getPrecursor(int index) const { return state(index + 1); }
        // Getters para V&V Etapa 2
        double getIodineConcentration() const { return state(7); }
        double getXenonConcentration() const { return state(8); }

    private:
        // Vector de estado: 
        // [0] = Potencia, [1..6] = Precursores, 
        // [7] = Yodo, [8] = Xenón, [9] = Prometio, [10] = Samario
        Eigen::VectorXd state;        

        double current_reactivity;   
        double t_fuel;             
        double t_coolant;          
        double base_temperature;      

        Eigen::VectorXd computeDerivatives(const Eigen::VectorXd& current_state, double effective_rho);
    };

} // namespace Yaxkin

#endif