#  YAXKIN: Digital Twin & Intelligent Cybersecurity Shield

**YAXKIN** (del Maya: *Sol Nuevo*) es un proyecto de ingeniería de software enfocado en la creación de un **Gemelo Digital** de un reactor nuclear para la investigación de **Ciberseguridad Industrial** e **Inteligencia Artificial**.

##  Arquitectura
- **The Core:** Motor de física neutrónica en C++.
- **The Brain:** Modelos LSTM/Autoencoders para detección de anomalías (Python/TensorFlow).
- **The Shield:** Red industrial emulada (Modbus/OPC UA) con protección IDS.

##  Cronograma (24 Meses)
- **Mes 1-6:** Modelado físico y motor C++.
- **Mes 7-12:** Microservicios y persistencia de datos (PostgreSQL).
- **Mes 13-18:** Entrenamiento de IA y Dashboard (React/Flutter).
- **Mes 19-24:** Pentesting, validación y tesis final.

---
**Desarrollado por:** Luis Alejandro Puebla Aguilar  
*Software Engineering Student | Robotics & Cybersecurity Enthusiast*
# YAXKIN-Nuclear


---

## Roadmap de Desarrollo: YAXKIN Digital Twin

### Etapa 1: El Núcleo Físico y Termodinámico
*El objetivo de esta fase es lograr que el motor de C++ sea matemáticamente estable y reaccione a las leyes de la física sin romperse.*

* **Paso 1: Estabilidad Numérica Avanzada.** Cambiar el método de Euler actual por un integrador **Runge-Kutta de cuarto orden (RK4)**. Esto eliminará los saltos violentos de potencia y permitirá simular transitorios rápidos con alta precisión.
* **Paso 2: Acoplamiento Termohidráulico.** Implementar las ecuaciones de transferencia de calor. La potencia generada calentará el combustible, el combustible calentará el agua, y el cambio de densidad del agua frenará la reacción neutrónica (**Coeficiente Doppler y de Moderador**).
* **Paso 3: Envenenamiento por Xenón y Samario.** Añadir la dinámica de los subproductos de fisión. El **Xenón-135** actúa como un veneno que absorbe neutrones horas después de apagar el reactor. Programar esto demuestra un entendimiento profundo de la ingeniería de reactores.

### Etapa 2: Interoperabilidad y Persistencia de Datos
*El motor no puede vivir aislado. Necesita transmitir sus datos al mundo exterior con latencias de milisegundos.*

* **Paso 1: Comunicación de Alta Frecuencia.** Integrar una librería de mensajería como **ZeroMQ o gRPC** en el código C++. El motor actuará como un publicador, enviando arreglos de datos (potencia, temperatura, presión) 10 o 20 veces por segundo.
* **Paso 2: Almacenamiento de Series de Tiempo.** Levantar un contenedor Docker con **PostgreSQL** y la extensión **TimescaleDB**. Esto permitirá guardar millones de puntos de datos históricos, lo cual es fundamental para entrenar la IA más adelante.
* **Paso 3: API Intermediaria.** Desarrollar un servicio en **Python o Node.js** que escuche a ZeroMQ y exponga los datos a través de **WebSockets** para que otras aplicaciones puedan consumirlos fácilmente.

### Etapa 3: Emulación de Red SCADA y Ciberseguridad
*Aquí es donde el simulador se convierte en un Cyber-Range. Replicaremos la vulnerabilidad de las redes industriales reales.*

* **Paso 1: Traducción a Protocolos Industriales.** Escribir scripts que tomen los datos del API y los mapeen a registros de **Modbus TCP** o variables de **OPC UA**. Tu reactor ahora hablará el mismo idioma que un PLC de Siemens o Allen-Bradley.
* **Paso 2: Sistema Inteligente de Detección de Intrusiones (AI-IDS).** Construir y entrenar un modelo de aprendizaje automático no supervisado (como **Autoencoders o LSTM**) utilizando los datos históricos. La IA aprenderá la correlación física entre variables para detectar alteraciones.
* **Paso 3: Ejercicios de Pentesting.** Utilizar herramientas de auditoría para interceptar y alterar el tráfico Modbus en la red local. El objetivo final será demostrar que la IA puede detectar un ataque de inyección de datos que burle los límites tradicionales de seguridad.

### Etapa 4: Interfaz Hombre-Máquina (HMI)
*La presentación visual de un sistema crítico debe ser prístina, funcional y libre de distracciones.*

* **Paso 1: Panel de Control en Tiempo Real.** Desarrollar una aplicación web (**React o Vue**) con una estética oscura y minimalista. Graficar la potencia y la temperatura en tiempo real utilizando librerías de alto rendimiento.
* **Paso 2: Controles Interactivos y Alarmas.** Integrar controles de mando bidireccionales, permitiendo al usuario mover las barras de control virtuales desde el navegador y ver la respuesta instantánea, junto con un panel de registro de eventos y alertas del sistema IDS.


---

# Plan de Verificación y Validación Matemática (V&V)
**Proyecto:** YAXKIN Digital Twin (SMR PWR)  
**Entorno:** C++17 / Eigen 3  
**Objetivo:** Garantizar la fidelidad física y estabilidad numérica del motor de simulación mediante la comparación con soluciones analíticas.

---

## Introducción
En sistemas de misión crítica, como un reactor nuclear, no es suficiente con que el código compile y ejecute. Es imperativo demostrar que los algoritmos numéricos (RK4 y Euler) convergen a las soluciones exactas de las ecuaciones diferenciales que rigen la física del núcleo. Este plan detalla los pasos para verificar de forma aislada cada componente del simulador.

---

## Etapa 1: Verificación del Estado Estacionario (Equilibrio Algebraico)
El objetivo es comprobar que el vector de estado inicial representa un equilibrio matemático perfecto. En ausencia de reactividad externa, las derivadas de todas las variables deben ser idénticamente cero.

* Paso 1: Equilibrio de Precursores. Calcular manualmente la concentración inicial de los 6 grupos de neutrones retardados. Para un equilibrio a potencia nominal $n_0 = 1.0$, la fórmula debe satisfacer:
  $$C_{i, 0} = \frac{\beta_i n_0}{\lambda_i \Lambda}$$
* Paso 2: Equilibrio de Venenos. Validar las concentraciones asintóticas de Yodo-135 y Xenón-135 igualando sus ecuaciones diferenciales a cero bajo flujo nominal:
  $$I_{eq} = \frac{\gamma_I \Sigma_f \phi_0}{\lambda_I}$$
  $$Xe_{eq} = \frac{(\gamma_{Xe} + \gamma_I) \Sigma_f \phi_0}{\lambda_{Xe} + \sigma_{a,Xe} \phi_0}$$
* Paso 3: Prueba de Deriva Nula (Zero-Drift). Ejecutar el simulador durante 10,000 pasos de tiempo sin insertar reactividad externa. El éxito se define como una variación menor al 0.001% en el vector de estado.

---

## Etapa 2: Validación de la Cinética Pura (Ecuación de Inhour)
Esta etapa aísla el motor neutrónico apagando temporalmente los módulos de calor y venenos para evaluar la respuesta a transitorios rápidos.

* Paso 1: Aislamiento del Sistema. Desactivar los feedbacks de temperatura y envenenamiento en la función `update`.
* Paso 2: Inserción de Reactividad en Escalón. Introducir un salto constante de reactividad ($\rho$), por ejemplo $\rho = 0.001$.
* Paso 3: Medición del Período Experimental. Registrar el tiempo que tarda la potencia en incrementarse por un factor de $e \approx 2.718$. Este valor es el período del reactor ($T$).
* Paso 4: Comprobación por Ecuación de Inhour. Comparar el período medido contra el período teórico resuelto mediante la relación de Inhour:
  $$\rho = \frac{\Lambda}{T} + \sum_{i=1}^{6} \frac{\beta_i}{1 + \lambda_i T}$$

---

## Etapa 3: Validación Termodinámica (Balance de Energía)
Se busca asegurar que el sistema cumpla con la Primera Ley de la Termodinámica y que el acoplamiento entre el combustible y el refrigerante sea estable.

* Paso 1: Estado de Potencia Constante. Forzar la variable de potencia a un valor estático (ej. 300 MW) para observar la respuesta térmica pura.
* Paso 2: Límite Teórico de Temperatura. Calcular el incremento de temperatura esperado en el refrigerante basado en el flujo másico y el calor específico:
  $$\Delta T = \frac{Q}{\dot{m} C_p}$$
* Paso 3: Verificación de Convergencia Térmica. Confirmar que la variable `t_coolant` converge exactamente al valor teórico calculado. Una desviación mayor a 0.5 °C indica un error en la discretización del tiempo o en las constantes de transferencia de calor.

---

## Etapa 4: Dinámica del Pozo de Yodo (Análisis Post-Apagado)
Evaluar la precisión de las ecuaciones de venenos durante transitorios de largo plazo (horas/días).

* Paso 1: Simulación de SCRAM. Correr el reactor en estado crítico y proceder a una inserción masiva de reactividad negativa para llevar la potencia a cero de forma instantánea.
* Paso 2: Monitoreo del Pico de Xenón. Registrar la acumulación de Xenón-135 post-apagado. El pico máximo de veneno debe ocurrir teóricamente entre las 9 y 11 horas después del cese de la fisión.
* Paso 3: Comparación con Solución Analítica de EDOs. Resolver manualmente la cadena de decaimiento $I \rightarrow Xe \rightarrow Stable$ y superponer la curva analítica con los datos exportados del simulador:
  $$Xe(t) = Xe_0 e^{-\lambda_{Xe} t} + \frac{\lambda_I I_0}{\lambda_{Xe} - \lambda_I} (e^{-\lambda_I t} - e^{-\lambda_{Xe} t})$$

---

## Criterios de Aceptación
Para que el Gemelo Digital sea considerado válido para investigación, todos los resultados de simulación deben mantener un margen de error relativo menor al 1% respecto a sus contrapartes analíticas en condiciones ideales.

---

