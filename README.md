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

## 🗺️ Roadmap de Desarrollo: YAXKIN Digital Twin

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

