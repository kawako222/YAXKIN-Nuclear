#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include "ReactorCore.hpp"

// =============================================================================
//  YAXKIN — Runner Principal
//
//  Modos disponibles:
//    [1] Simulación de operación normal (escalón de reactividad)
//    [2] V&V 1: Equilibrio de precursores
//    [3] V&V 2: Equilibrio de venenos (Xe-135 / I-135)
//    [4] V&V 3: Prueba de deriva nula (Zero-Drift)
//    [5] V&V 4: Período del reactor (cinética pura)
// =============================================================================

namespace Yaxkin::Tests {

// -----------------------------------------------------------------------------
// [1] Simulación normal — rampa de reactividad y registro completo de estado
// -----------------------------------------------------------------------------
void runNormalSimulation() {
    std::cout << "\n╔══════════════════════════════════════════════════╗\n";
    std::cout <<   "║        YAXKIN-1  —  SIMULACIÓN NORMAL            ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════╝\n\n";

    ReactorCore reactor;
    const double dt         = 0.1;
    const double total_time = 10.0;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::left
              << std::setw(10) << "Tiempo(s)"
              << std::setw(12) << "Potencia"
              << std::setw(12) << "T_Fuel(C)"
              << std::setw(14) << "T_Coolant(C)"
              << std::setw(12) << "Xe-135"
              << "Reactividad\n";
    std::cout << std::string(72, '-') << "\n";

    for (double t = 0; t <= total_time; t += dt) {
        // Escalón de reactividad: +100 pcm entre t=1s y t=2s
        const double rho_input = (t > 1.0 && t < 2.0) ? 0.001 : 0.0;
        reactor.update(dt, rho_input);

        std::cout << std::setw(10) << t
                  << std::setw(12) << std::setprecision(6) << reactor.getPower()
                  << std::setw(12) << std::setprecision(2) << reactor.getFuelTemp()
                  << std::setw(14) << reactor.getCoolantTemp()
                  << std::setw(12) << std::scientific << std::setprecision(3) << reactor.getXenonConcentration()
                  << std::fixed   << std::setprecision(5) << reactor.getReactivity() << "\n";
    }
}

// -----------------------------------------------------------------------------
// [2] V&V 1: Equilibrio de precursores en t=0
// Verifica que Ci(0) = β_i·n₀ / (λ_i·Λ)
// -----------------------------------------------------------------------------
void runVV_Precursors() {
    std::cout << "\n╔══════════════════════════════════════════════════╗\n";
    std::cout <<   "║   V&V ETAPA 1 — EQUILIBRIO DE PRECURSORES        ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════╝\n\n";

    ReactorCore reactor;

    std::cout << std::fixed << std::setprecision(4);
    std::cout << std::left
              << std::setw(8)  << "Grupo"
              << std::setw(18) << "Esperado"
              << std::setw(18) << "Simulador"
              << "Error rel.\n";
    std::cout << std::string(50, '-') << "\n";

    bool allPass = true;
    for (int i = 0; i < 6; ++i) {
        double expected = (NuclearConstants::BETA_I[i] * 1.0) /
                          (NuclearConstants::LAMBDA_I[i] * NuclearConstants::PROMPT_GEN_TIME);
        double simVal   = reactor.getPrecursor(i);
        double err      = std::abs(simVal - expected) / expected;
        bool   pass     = err < 1e-6;
        if (!pass) allPass = false;

        std::cout << std::setw(8)  << (i + 1)
                  << std::setw(18) << std::scientific << expected
                  << std::setw(18) << simVal
                  << std::fixed    << std::setprecision(2) << (err * 100.0) << "%"
                  << (pass ? "  ✓" : "  ✗ FALLO") << "\n";
    }

    std::cout << "\n[RESULTADO] " << (allPass ? "EXITOSO — todos los precursores en equilibrio." 
                                              : "FALLIDO — revisar constantes.") << "\n";
}

// -----------------------------------------------------------------------------
// [3] V&V 2: Equilibrio de venenos (Xe-135 / I-135)
// -----------------------------------------------------------------------------
void runVV_Poisons() {
    std::cout << "\n╔══════════════════════════════════════════════════╗\n";
    std::cout <<   "║   V&V ETAPA 2 — EQUILIBRIO DE VENENOS            ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════╝\n\n";

    ReactorCore reactor;

    // Valores analíticos de referencia (phi = NOMINAL_FLUX × 1.0)
    const double phi         = NuclearConstants::NOMINAL_FLUX;
    const double expected_I  = (NuclearConstants::YIELD_IODINE * NuclearConstants::SIGMA_F * phi) /
                                NuclearConstants::LAMBDA_IODINE;
    const double expected_Xe = (NuclearConstants::YIELD_XE * NuclearConstants::SIGMA_F * phi +
                                NuclearConstants::LAMBDA_IODINE * expected_I) /
                               (NuclearConstants::LAMBDA_XE + NuclearConstants::SIGMA_A_XE * phi);

    double err_I  = std::abs(reactor.getIodineConcentration()  - expected_I)  / expected_I;
    double err_Xe = std::abs(reactor.getXenonConcentration() - expected_Xe) / expected_Xe;

    std::cout << std::scientific << std::setprecision(4);
    std::cout << "Yodo-135\n"
              << "  Esperado  : " << expected_I  << "\n"
              << "  Simulador : " << reactor.getIodineConcentration() << "\n"
              << "  Error rel.: " << std::fixed << std::setprecision(4) << (err_I * 100) << "%"
              << (err_I < 0.001 ? "  ✓\n" : "  ✗ FALLO\n");

    std::cout << "\nXenón-135\n";
    std::cout << std::scientific << std::setprecision(4);
    std::cout << "  Esperado  : " << expected_Xe << "\n"
              << "  Simulador : " << reactor.getXenonConcentration() << "\n"
              << "  Error rel.: " << std::fixed << std::setprecision(4) << (err_Xe * 100) << "%"
              << (err_Xe < 0.001 ? "  ✓\n" : "  ✗ FALLO\n");

    std::cout << "\n[RESULTADO] "
              << ((err_I < 0.001 && err_Xe < 0.001)
                  ? "EXITOSO — venenos en equilibrio. Error < 0.1%"
                  : "FALLIDO — desviación detectada. Revisar constantes.")
              << "\n";
}

// -----------------------------------------------------------------------------
// [4] V&V 3: Zero-Drift — estabilidad numérica en 100 segundos sin reactividad
// -----------------------------------------------------------------------------
void runVV_ZeroDrift() {
    std::cout << "\n╔══════════════════════════════════════════════════╗\n";
    std::cout <<   "║   V&V ETAPA 3 — PRUEBA DE DERIVA NULA            ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════╝\n\n";

    ReactorCore reactor;
    const double dt         = 0.1;
    const double total_time = 100.0;
    const int    steps      = static_cast<int>(total_time / dt);
    const double p0         = reactor.getPower();

    std::cout << "Simulando " << total_time << "s de estabilidad (sin reactividad externa)...\n\n";
    std::cout << std::left
              << std::setw(10) << "Tiempo(s)"
              << std::setw(14) << "Potencia"
              << std::setw(12) << "T_Fuel(C)"
              << "T_Coolant(C)\n";
    std::cout << std::string(48, '-') << "\n";

    for (int i = 0; i <= steps; ++i) {
        if (i % 100 == 0) {
            std::cout << std::fixed << std::setprecision(2)
                      << std::setw(10) << (i * dt)
                      << std::setw(14) << std::setprecision(8) << reactor.getPower()
                      << std::setw(12) << std::setprecision(2) << reactor.getFuelTemp()
                      << reactor.getCoolantTemp() << "\n";
        }
        reactor.update(dt, 0.0);
    }

    const double drift = std::abs(reactor.getPower() - p0);
    std::cout << std::string(48, '-') << "\n";
    std::cout << "Deriva total tras " << total_time << "s: " 
              << std::scientific << std::setprecision(3) << drift << "\n";
    std::cout << "[RESULTADO] "
              << (drift < 1e-5 ? "EXITOSO — núcleo matemáticamente estable."
                               : "FALLIDO — deriva numérica detectada. Revisar constantes.")
              << "\n";
}

// -----------------------------------------------------------------------------
// [5] V&V 4: Período del reactor — mide T experimental y compara con Inhour
// Un escalón de +100 pcm debería dar T ≈ 80–100 s para un PWR típico
// -----------------------------------------------------------------------------
void runVV_ReactorPeriod() {
    std::cout << "\n╔══════════════════════════════════════════════════╗\n";
    std::cout <<   "║   V&V ETAPA 4 — PERÍODO DEL REACTOR              ║\n";
    std::cout <<   "╚══════════════════════════════════════════════════╝\n\n";

    // NOTA: usamos updateIsothermal() — reactor sin feedback de temperatura
    // ni venenos. Esto es correcto para medir cinética puntual pura.
    // En el reactor real el feedback frenaría el crecimiento, pero la ecuación
    // de Inhour que usaremos para validar asume igualmente condiciones isotérmicas.
    ReactorCore reactor;
    const double dt              = 0.01;
    const double step_reactivity = 0.001; // 100 pcm
    const double target_power    = std::exp(1.0); // n₀ × e (un período T)
    double time = 0.0;

    std::cout << "Potencia inicial       : 1.0000\n"
              << "Potencia objetivo (×e) : " << std::fixed << std::setprecision(5) << target_power << "\n"
              << "Reactividad escalón    : " << step_reactivity << " Δk/k  (100 pcm)\n"
              << "Modo                   : Isotérmico (sin feedback)\n\n"
              << "Midiendo período del reactor...\n";

    while (reactor.getPower() < target_power) {
        reactor.updateIsothermal(dt, step_reactivity);
        time += dt;
        if (time > 2000.0) {
            std::cout << "[ERROR] Tiempo de simulación excedido (>2000s).\n";
            return;
        }
    }

    std::cout << "\n[RESULTADO EXPERIMENTAL]\n"
              << "Período T medido      : " << std::fixed << std::setprecision(3) << time << " s\n"
              << "\n[VALIDACIÓN — Ecuación de Inhour]\n"
              << "  Calcula: ρ = Λ/T + Σ [ β_i / (1 + λ_i·T) ]\n"
              << "  Con T = " << std::setprecision(3) << time << " s\n"
              << "  El ρ resultante debe ser ≈ " << step_reactivity << " (±5%)\n";

    // Validación inline con Inhour
    double rho_inhour = NuclearConstants::PROMPT_GEN_TIME / time;
    for (int i = 0; i < 6; ++i) {
        rho_inhour += NuclearConstants::BETA_I[i] / (1.0 + NuclearConstants::LAMBDA_I[i] * time);
    }
    double err = std::abs(rho_inhour - step_reactivity) / step_reactivity;
    std::cout << "  ρ (Inhour)            : " << std::scientific << std::setprecision(5) << rho_inhour << "\n"
              << "  Error relativo        : " << std::fixed << std::setprecision(2) << (err * 100.0) << "%\n"
              << "\n[RESULTADO] "
              << (err < 0.05 ? "EXITOSO — período consistente con Inhour (±5%)."
                             : "ATENCIÓN — desviación >5%. Revisar Λ o β_i.")
              << "\n";
}

} // namespace Yaxkin::Tests

// =============================================================================
//  MENÚ PRINCIPAL — ciclo continuo hasta que el usuario elija salir
// =============================================================================
static void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

static void printBanner() {
    std::cout << "\n";
    std::cout << "  ██╗   ██╗ █████╗ ██╗  ██╗██╗  ██╗██╗███╗   ██╗\n";
    std::cout << "  ╚██╗ ██╔╝██╔══██╗╚██╗██╔╝██║ ██╔╝██║████╗  ██║\n";
    std::cout << "   ╚████╔╝ ███████║ ╚███╔╝ █████╔╝ ██║██╔██╗ ██║\n";
    std::cout << "    ╚██╔╝  ██╔══██║ ██╔██╗ ██╔═██╗ ██║██║╚██╗██║\n";
    std::cout << "     ██║   ██║  ██║██╔╝ ██╗██║  ██╗██║██║ ╚████║\n";
    std::cout << "     ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝\n";
    std::cout << "          Digital Twin — Nuclear Reactor Core v0.3\n\n";
}

static void printMenu() {
    std::cout << "  Selecciona un modo:\n\n";
    std::cout << "  [1] Simulación de operación normal\n";
    std::cout << "  [2] V&V 1 — Equilibrio de precursores\n";
    std::cout << "  [3] V&V 2 — Equilibrio de venenos (Xe-135 / I-135)\n";
    std::cout << "  [4] V&V 3 — Prueba de deriva nula (Zero-Drift)\n";
    std::cout << "  [5] V&V 4 — Período del reactor (cinética pura)\n";
    std::cout << "  [0] Ejecutar toda la batería V&V\n";
    std::cout << "  [q] Salir\n\n";
    std::cout << "  Opción: ";
}

int main() {
    clearScreen();
    printBanner();

    std::string input;
    while (true) {
        printMenu();
        std::cin >> input;

        if (input == "q" || input == "Q") {
            clearScreen();
            std::cout << "  YAXKIN — Sesión terminada.\n\n";
            break;
        }

        int choice = -1;
        try { choice = std::stoi(input); } catch (...) { choice = -1; }

        clearScreen();
        printBanner();

        switch (choice) {
            case 1: Yaxkin::Tests::runNormalSimulation(); break;
            case 2: Yaxkin::Tests::runVV_Precursors();    break;
            case 3: Yaxkin::Tests::runVV_Poisons();       break;
            case 4: Yaxkin::Tests::runVV_ZeroDrift();     break;
            case 5: Yaxkin::Tests::runVV_ReactorPeriod(); break;
            case 0:
                Yaxkin::Tests::runVV_Precursors();
                Yaxkin::Tests::runVV_Poisons();
                Yaxkin::Tests::runVV_ZeroDrift();
                Yaxkin::Tests::runVV_ReactorPeriod();
                break;
            default:
                std::cout << "\n  [!] Opción no válida. Intenta de nuevo.\n";
                break;
        }

        std::cout << "\n  Presiona Enter para volver al menú...";
        std::cin.ignore();
        std::cin.get();
        clearScreen();
        printBanner();
    }

    return 0;
}