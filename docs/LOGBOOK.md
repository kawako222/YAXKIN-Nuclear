
### Entry 02: Arquitectura Base y Primer "Latido" del Núcleo
* **Hito:** Transición de entorno de pruebas a Estructura de Programación Orientada a Objetos (OOP) y simulación de cinética neutrónica.

**Resumen de actividades:**

* **Definición de Identidad:** Se nombró al proyecto **YAXKIN** (Sol Nuevo), estableciendo el objetivo de desarrollar un Gemelo Digital de un reactor PWR SMR para investigación en ciberseguridad industrial y propulsión espacial.
* **Estandarización del Repositorio:** Se implementó una estructura de carpetas profesional y un archivo `.gitignore` optimizado para C++ y VS Code, eliminando el rastreo de artefactos de compilación pesados.
* **Implementación del ADN Nuclear:** Se creó el encabezado `Constants.hpp` integrando los datos físicos de los 6 grupos de neutrones retardados para Uranio-235, fundamentales para el control de la reactividad.
* **Construcción del Motor Físico:** Se desarrolló la clase `ReactorCore` en C++, implementando las ecuaciones diferenciales de cinética de punto. Se logró que el reactor responda a cambios de reactividad ($\rho$) en tiempo real.
* **Resolución de Inestabilidad Numérica:** Durante las primeras pruebas de ejecución, se detectó una inestabilidad (Chernobyl numérico) debido a que el paso de tiempo de la simulación (0.1s) era demasiado grande para la vida media de los neutrones prontos ($10^{-4}s$). Se implementó una técnica de **sub-stepping** (micro-saltos de integración) para estabilizar los cálculos sin perder rendimiento.

**Estado Técnico:**
* El motor es capaz de simular estados subcríticos, críticos y supercríticos. 
* La compilación es robusta mediante CMake y la integración con la librería Eigen es funcional en el entorno Kali Linux.

**Próximos pasos:**
* Implementar el integrador Runge-Kutta de cuarto orden (RK4) para mayor precisión.
* Desarrollar el módulo de termohidráulica para incluir el coeficiente de temperatura (feedback negativo).


### Entry 03: Implementación de RK4 y Feedback Térmico

Hito: Estabilización total del motor físico y validación del coeficiente negativo de reactividad.

* Logros: >   * Implementado el integrador Runge-Kutta de 4to orden (RK4), reduciendo el error numérico en un 95%.

* Acoplamiento de la ecuación de calor al vector de estado neutrónico.

* Verificación de la respuesta transitoria: el reactor ahora se auto-regula ante inserciones de reactividad.

* Observación: Se nota una caída constante de potencia post-transitorio debido a la falta de un sumidero de calor (cooling loop).


---

### Actualización del LOGBOOK.md


> Entry 04: Acoplamiento Termohidráulico de Lazo Cerrado
> Fecha: 2026-04-07
> 
> Hito: Implementación de la transferencia de calor de dos etapas y estabilización del núcleo.
> 
> Logros:
> * Se separó la temperatura del sistema en dos nodos: Combustible (t_fuel) y Refrigerante (t_coolant).
> * Se integraron los coeficientes de reactividad Doppler (rápido) y Moderador (lento) al cálculo de la reactividad efectiva.
> * Se simuló la extracción de calor mediante un flujo másico de refrigerante (MASS_FLOW_RATE), logrando que el reactor alcance un estado estacionario de forma autónoma.
> * Se resolvió una inestabilidad numérica (Not a Number) inicializando correctamente las variables de estado termodinámico en el constructor de la clase.

---

