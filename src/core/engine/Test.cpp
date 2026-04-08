
    ////////////////////////VALIDACION MATEMATICA
    /*
    std::cout << "--- V&V ETAPA 1: EQUILIBRIO DE PRECURSORES ---\n";
    
    // Instanciamos el reactor (esto llama al constructor que queremos validar)
    Yaxkin::ReactorCore reactor;

    // Imprimimos los valores generados por el código
    std::cout << std::fixed << std::setprecision(4);
    for (int i = 0; i < 6; ++i) {
        std::cout << "Grupo " << (i + 1) << " | Valor en simulador: " << reactor.getPrecursor(i) << "\n";
    }

    return 0;
    */
    /*
    std::cout << "--- V&V ETAPA 2: EQUILIBRIO DE VENENOS (Xe-135 / I-135) ---\n";
    
    Yaxkin::ReactorCore reactor;

    std::cout << std::scientific << std::setprecision(4);
    std::cout << "Yodo-135  | Esperado: 3.3397e+15 | En simulador: " << reactor.getIodineConcentration() << "\n";
    std::cout << "Xenon-135 | Esperado: 9.9009e+14 | En simulador: " << reactor.getXenonConcentration() << "\n";

    // Verificación de error relativo
    double err_xe = std::abs(reactor.getXenonConcentration() - 9.9009e14) / 9.9009e14;
    if (err_xe < 0.001) {
        std::cout << "\n[RESULTADO] Verificacion EXITOSA. Error relativo < 0.1%\n";
    } else {
        std::cout << "\n[RESULTADO] ERROR: Desviacion detectada en la concentracion.\n";
    }

    return 0;
    */
   /*
    Yaxkin::ReactorCore reactor;
    double dt = 0.1; // Pasos de 100ms
    double total_time = 100.0; // Simularemos 100 segundos de operacion real
    int steps = static_cast<int>(total_time / dt);

    std::cout << "--- V&V ETAPA 1.3: PRUEBA DE DERIVA NULA (ZERO-DRIFT) ---\n";
    std::cout << "Simulando " << total_time << " segundos de estabilidad...\n\n";

    double initial_power = reactor.getPower();
    
    std::cout << "Tiempo | Potencia | T_Fuel | T_Coolant\n";
    std::cout << "------------------------------------------\n";

    for (int i = 0; i <= steps; ++i) {
        // Imprimimos cada 10 segundos para no saturar la consola
        if (i % 100 == 0) {
            std::cout << std::fixed << std::setprecision(2) << (i * dt) << "s    | "
                      << std::setprecision(6) << reactor.getPower() << " | "
                      << std::setprecision(2) << reactor.getFuelTemp() << "C | "
                      << reactor.getCoolantTemp() << "C\n";
        }

        // Ejecutamos la actualizacion SIN reactividad externa
        reactor.update(dt, 0.0);
    }

    double final_power = reactor.getPower();
    double drift = std::abs(final_power - initial_power);

    std::cout << "------------------------------------------\n";
    std::cout << "Deriva total despues de 100s: " << std::scientific << drift << "\n";

    if (drift < 1e-5) {
        std::cout << " El nucleo es matematicamente estable.\n";
    } else {
        std::cout << " FALLO: Se detecto deriva numerica. Revisar constantes.\n";
    }

    return 0;
    */
    /*
    //////////////////// Fase 2 comprobacion de ecuación cinética pura /////////////////
    Yaxkin::ReactorCore reactor;
    
    // Usamos un paso de tiempo más fino (10ms) para capturar el instante exacto
    double dt = 0.01; 
    double time = 0.0;
    
    // La potencia inicial es 1.0. Buscamos cuando se multiplique por el número de Euler (e)
    double target_power = std::exp(1.0); 
    
    // Paso 2: Inserción de Reactividad en Escalón (100 pcm)
    double step_reactivity = 0.001; 

    std::cout << "--- V&V ETAPA 2: CINETICA PURA (PERIODO DEL REACTOR) ---\n";
    std::cout << "Potencia Inicial: 1.0000\n";
    std::cout << "Potencia Objetivo (1.0 * e): " << std::fixed << std::setprecision(5) << target_power << "\n";
    std::cout << "Insertando escalon de reactividad: " << step_reactivity << "...\n\n";

    // Paso 3: Medición Experimental
    while (reactor.getPower() < target_power) {
        reactor.update(dt, step_reactivity);
        time += dt;

        // Failsafe matemático por si el reactor se vuelve supercrítico pronto y explota en ms
        if (time > 1000.0) {
            std::cout << "[ERROR] Tiempo de simulacion excedido.\n";
            break;
        }
    }

    std::cout << "[RESULTADO EXPERIMENTAL]\n";
    std::cout << "El Periodo del Reactor (T) medido en el motor C++ es: " 
              << std::fixed << std::setprecision(4) << time << " segundos.\n";
              
    std::cout << "\nGuarda este valor de T. Lo usaremos en la Ecuacion de Inhour para el Paso 4.\n";

    return 0;
    */