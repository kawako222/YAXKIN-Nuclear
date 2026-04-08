

## Manual del Desarrollador: Proyecto YAXKIN

---

### 1. Workflow de Desarrollo (VS Code + CMake)
Para trabajar en este proyecto, el ciclo siempre es:
1.  **Modificar código:** Editas tus archivos `.cpp` o `.hpp`.
2.  **Build (`F7`):** Compila los cambios. Si hay errores de sintaxis, aquí aparecerán.
3.  **Run (`Shift + F5`):** Ejecuta el motor para ver los resultados en la terminal.
4.  **Debug (`F5`):** Úsalo si el programa "truena" o si los cálculos físicos no dan lo que esperabas.

### 2. Comandos de Emergencia (Terminal Kali)
Si VS Code se pone caprichoso, estos comandos nunca fallan:
```bash
cd build
cmake ..   # Solo si agregaste archivos nuevos al proyecto
make       # Compila
./yaxkin_core  # Ejecuta
```

---

## Actualización del LOGBOOK.md

Como hablamos de llevar una bitácora, crea (o actualiza) tu archivo `docs/LOGBOOK.md` con esta primera entrada técnica. Esto es lo que leerán los evaluadores para ver tu progreso:

> **Entry 01: Setup y Validación del Entorno**
> * **Fecha:** 2026-04-05
> * **Hito:** Configuración exitosa del motor de simulación en C++17 sobre Kali Linux.
> * **Logros:**
>   * Integración de la librería **Eigen 3** para cálculo matricial de alta densidad.
>   * Implementación de un sistema de construcción robusto mediante **CMake**.
>   * Verificación del flujo de datos inicial mediante un vector de estado del reactor.
> * **Notas Técnicas:** Se resolvió conflicto de *includePath* vinculando la ruta `/usr/include/eigen3` en la configuración de CMake.

---

##  Siguiente Paso: El ADN del Reactor

Ahora que el entorno es sólido como una roca, necesitamos que **YAXKIN** sepa qué es un reactor. Vamos a crear el archivo de **Constantes Nucleares** que te mencioné. Esto es pura física aplicada al código.

**Crea el archivo `src/core/include/Constants.hpp`:**

```cpp
#ifndef YAXKIN_CONSTANTS_HPP
#define YAXKIN_CONSTANTS_HPP

#include <vector>

namespace Yaxkin {

    /**
     * @brief Datos físicos para un reactor térmico de U-235.
     * Estos valores definen la dinámica de la población de neutrones.
     */
    struct NuclearConstants {
        // Lambda (s): Tiempo de generación de neutrones prontos
        static constexpr double PROMPT_GEN_TIME = 1e-4; 
        
        // Beta Total: Fracción de neutrones que no son instantáneos
        static constexpr double TOTAL_BETA = 0.0065;

        // Grupos de neutrones retardados (6 grupos estándar)
        // beta_i: Fracción de cada grupo
        inline static const std::vector<double> BETA_I = {
            0.000215, 0.001424, 0.001274, 0.002568, 0.000748, 0.000273
        };

        // lambda_i: Constantes de decaimiento en s^-1
        inline static const std::vector<double> LAMBDA_I = {
            0.0124, 0.0305, 0.111, 0.301, 1.14, 3.01
        };
    };

} // namespace Yaxkin

#endif
```
