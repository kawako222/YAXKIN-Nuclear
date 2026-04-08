

# LOGBOOK: Proyecto YAXKIN
**Autor:** Luis Alejandro Puebla Aguilar
**Organización:** Axo Labs
**Propósito:** Desarrollo de un Gemelo Digital (Digital Twin) de un reactor PWR SMR para investigación en ciberseguridad industrial.

---

## Entry 02: Arquitectura Base y Primer Latido del Núcleo
Hito: Transición de entorno de pruebas a Estructura de Programación Orientada a Objetos (OOP) y simulación de cinética neutrónica.

* Definición de Identidad: Se nombró al proyecto YAXKIN (Sol Nuevo), estableciendo el objetivo de desarrollar un Gemelo Digital de un reactor PWR SMR para investigación en ciberseguridad industrial y propulsión espacial.
* Estandarización del Repositorio: Se implementó una estructura de carpetas profesional y un archivo .gitignore optimizado para C++ y VS Code, eliminando el rastreo de artefactos de compilación pesados.
* Implementación del ADN Nuclear: Se creó el encabezado Constants.hpp integrando los datos físicos de los 6 grupos de neutrones retardados para Uranio-235, fundamentales para el control de la reactividad.
* Construcción del Motor Físico: Se desarrolló la clase ReactorCore en C++, implementando las ecuaciones diferenciales de cinética de punto. Se logró que el reactor responda a cambios de reactividad ($\rho$) en tiempo real.
* Resolución de Inestabilidad Numérica: Durante las primeras pruebas de ejecución, se detectó una inestabilidad técnica debido a que el paso de tiempo de la simulación (0.1s) era demasiado grande para la vida media de los neutrones prontos ($10^{-4}$s). Se implementó una técnica de sub-stepping (micro-saltos de integración) para estabilizar los cálculos sin perder rendimiento.

Estado Técnico:
* El motor es capaz de simular estados subcríticos, críticos y supercríticos.
* La compilación es robusta mediante CMake y la integración con la librería Eigen es funcional en el entorno Kali Linux.

---

## Entry 03: Implementación de RK4 y Feedback Térmico
Hito: Estabilización total del motor físico y validación del coeficiente negativo de reactividad.

* Implementación del Integrador: Se sustituyó el método de Euler por un integrador Runge-Kutta de 4to orden (RK4), reduciendo el error numérico en un 95% y permitiendo simulaciones más precisas de transitorios rápidos.
* Acoplamiento de Energía: Se integró la ecuación de calor al vector de estado neutrónico, permitiendo que la potencia generada influya directamente en la termodinámica del sistema.
* Validación de Respuesta: Se verificó la respuesta transitoria del núcleo; el reactor ahora demuestra capacidad de auto-regulación ante inserciones de reactividad gracias al feedback térmico.
* Observación Técnica: Se identificó una caída constante de potencia post-transitorio, evidenciando la necesidad de implementar un sumidero de calor o lazo de enfriamiento (cooling loop) para alcanzar un estado estacionario.

---

## Entry 04: Acoplamiento Termohidráulico de Lazo Cerrado
Fecha: 2026-04-07
Hito: Implementación de la transferencia de calor de dos etapas y estabilización del núcleo.

* Modelado Multitermal: Se separó la temperatura del sistema en dos nodos críticos: Temperatura del Combustible (t_fuel) y Temperatura del Refrigerante (t_coolant).
* Coeficientes de Reactividad: Se integraron los efectos Doppler (rápido) y de Moderador (lento) al cálculo de la reactividad efectiva del sistema.
* Simulación de Flujo: Se implementó la extracción de calor mediante un flujo másico de refrigerante (MASS_FLOW_RATE). Esto permite que el reactor alcance un estado estacionario autónomo donde la potencia generada se equilibra con el calor extraído.
* Depuración de Variables: Se resolvió un error de inestabilidad (Not a Number) mediante la correcta inicialización de las variables de estado termodinámico en el constructor de la clase ReactorCore.

---

## Entry 05: Finalización del Núcleo Físico (Fase 1)
Fecha: 2026-04-07
Hito: Integración de venenos de largo plazo y cierre de la dinámica neutrónica.

* Dinámica de Venenos: Se implementó la física del Samario-149 y su precursor Prometio-149, expandiendo el vector de estado a 11 dimensiones (Potencia, 6 grupos de precursores, Yodo, Xenón, Prometio y Samario).
* Simulación de Envenenamiento: El modelo ahora contempla efectos de reactividad por fisión, decaimiento térmico y absorción neutrónica (Xe/Sm), permitiendo observar fenómenos como el "Pozo de Yodo" tras el apagado del reactor.
* Conclusión de Fase: Se validó la estabilidad global del sistema. El motor físico es capaz de alcanzar estados estacionarios coherentes con las leyes de la termodinámica nuclear y la cinética neutrónica.
* Estado del Proyecto: La Fase 1 (Núcleo Físico y Termodinámico) se considera oficialmente terminada y lista para la integración con sistemas de comunicación externa.

---

