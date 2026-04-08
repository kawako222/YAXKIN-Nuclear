#ifndef YAXKIN_REACTOR_CORE_HPP
#define YAXKIN_REACTOR_CORE_HPP

#include "Constants.hpp"
#include <Eigen/Dense>

namespace Yaxkin {

    class ReactorCore {
    public:
        ReactorCore();

        void update(double dt, double reactivity_input);

        // Variante isotérmica: congela temperatura y venenos en su valor inicial.
        // Usada exclusivamente en V&V 4 (período del reactor) para medir cinética
        // puntual pura sin que el feedback térmico oponga reactividad negativa.
        void updateIsothermal(double dt, double reactivity_input);

        double getFuelTemp()             const { return t_fuel; }
        double getCoolantTemp()          const { return t_coolant; }
        double getPower()                const { return state(0); }
        double getReactivity()           const { return current_reactivity; }
        double getPrecursor(int index)   const { return state(index + 1); }
        double getIodineConcentration()  const { return state(7); }
        double getXenonConcentration()   const { return state(8); }
        double getPromethiumConc()       const { return state(9); }
        double getSamariumConc()         const { return state(10); }

    private:
        // Vector de estado:
        // [0]    = Potencia normalizada
        // [1..6] = Concentración de precursores de neutrones retardados
        // [7]    = Yodo-135
        // [8]    = Xenón-135
        // [9]    = Prometio-149
        // [10]   = Samario-149
        Eigen::VectorXd state;

        double current_reactivity;
        double t_fuel;
        double t_coolant;
        double base_temperature;

        // FIX: Antes eran variables `static` dentro de update() — esto causaba
        // que se inicializaran una sola vez en todo el programa, rompiendo
        // cualquier simulación que no empezara desde el estado por defecto.
        // Ahora son miembros de instancia, inicializados correctamente en
        // el constructor con los valores de equilibrio real del reactor.
        double initial_xenon_rho;
        double initial_samarium_rho;

        Eigen::VectorXd computeDerivatives(const Eigen::VectorXd& current_state,
                                           double effective_rho);
    };

} // namespace Yaxkin

#endif