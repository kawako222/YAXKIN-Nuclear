#include <iostream>
#include <Eigen/Dense>

int main() {
    std::cout << "--- ☀️ YAXKIN NUCLEAR CORE STARTING ---" << std::endl;

    Eigen::VectorXd reactor_state(2);
    reactor_state << 100.0, 0.5; // 100% potencia, 0.5 concentración de neutrones retardados

    std::cout << "Estado inicial del reactor:\\n" << reactor_state << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "✅ Entorno configurado correctamente." << std::endl;

    return 0;
}