# IBin: Gestión Inteligente de Residuos IoT 🗑️🌐
IBin es un proyecto intermodular desarrollado para el ciclo de Sistemas Microinformáticos y Redes (SMR). Consiste en una papelera inteligente diseñada para mejorar la higiene y optimizar la gestión de residuos mediante el uso de tecnologías de Internet de las Cosas (IoT).

---

## 🚀 Características Principales

- **Apertura Touchless:** Apertura automática de la tapa mediante un sensor de presencia PIR para evitar el contacto físico.
- **Monitoreo de Nivel:** Medición en tiempo real del llenado mediante un sensor de ultrasonidos con media de 5 lecturas para mayor precisión.
- **Interfaz Visual LED:** Tres diodos LED (verde, naranja y rojo) que actúan como semáforo indicando la capacidad disponible.
- **Aviso de Cierre:** Parpadeo de todos los LEDs durante los últimos 5 segundos antes de cerrar la tapa.
- **Bloqueo por Llenado:** La tapa se bloquea automáticamente cuando el nivel de llenado supera el umbral máximo.
- **Conectividad Cloud:** Integración con la plataforma Blynk para supervisión remota desde dispositivos móviles.
- **Modo Noche:** Activación remota desde Blynk para bloquear la apertura automática durante horarios específicos.

---

## 🛠️ Hardware Utilizado

| Componente | Función |
|---|---|
| ESP32 DevKit V1 | Microcontrolador central con WiFi integrado. |
| HC-SR04 | Sensor de ultrasonidos para medir el nivel de llenado. |
| HC-SR501 (PIR) | Sensor infrarrojo para detectar movimiento y presencia. |
| Motor 28BYJ-48 + Driver ULN2003 | Actuador para el mecanismo de apertura de la tapa mediante hilo de pesca. |
| LEDs (x3) | Interfaz visual tipo semáforo (verde, naranja, rojo). |
| Protoboard | Distribución central de alimentación para todos los componentes. |
| Fuente 5V / 2A | Alimentación estable del sistema. |

---

## ⚙️ Mecanismo de Apertura

El motor paso a paso 28BYJ-48 lleva unido un soporte en su eje. A ese soporte se ancla un hilo de pesca de alta resistencia, cuyo otro extremo está fijado a un tornillo instalado en la tapa de la papelera.

- Cuando el motor ejecuta `motor.step(-700)`, el hilo se enrolla y tira de la tapa hacia abajo, **abriéndola**.
- Cuando el motor ejecuta `motor.step(600)`, el hilo se desenrolla y la tapa sube a su posición original, **cerrándola**.

---

## 🔧 Configuración y Montaje

### Conexiones (Pinout ESP32)

| Componente | Pin(es) ESP32 |
|---|---|
| Sensor PIR HC-SR501 | GPIO 26 |
| Ultrasonido TRIG | GPIO 5 |
| Ultrasonido ECHO | GPIO 18 |
| Motor ULN2003 IN1 | GPIO 13 |
| Motor ULN2003 IN2 | GPIO 14 |
| Motor ULN2003 IN3 | GPIO 12 |
| Motor ULN2003 IN4 | GPIO 27 |
| LED verde | GPIO 17 |
| LED naranja | GPIO 16 |
| LED rojo | GPIO 19 |
| VCC (todos) | VIN / 5V (protoboard) |
| GND (todos) | GND (protoboard) |

### Pines Virtuales Blynk

| Pin Virtual | Dato |
|---|---|
| V0 | Nivel de llenado (0-100%) |
| V1 | Estado tapa (0=cerrada, 1=abierta) |
| V2 | Switch modo noche (0=desactivado, 1=activado) |

### Lógica de niveles (ultrasonido)

| Distancia | Estado | LED |
|---|---|---|
| > 15 cm | Vacía | Verde |
| 5 - 15 cm | Media | Naranja |
| < 5 cm | Llena — tapa bloqueada | Rojo |

### Instalación de Librerías

Es necesario instalar las siguientes librerías en el IDE de Arduino:

- `Stepper` (incluida en el IDE de Arduino, no requiere instalación adicional)
- `BlynkSimpleEsp32`
- `WiFi` (incluida en el paquete ESP32 para Arduino)

> **Nota:** Las librerías `ESP32Servo` y `Adafruit NeoPixel` ya no son necesarias en esta versión del proyecto.

---

## 💡 Lógica de Funcionamiento

El sistema está basado en una **máquina de estados** con 4 estados:

| Estado | Descripción |
|---|---|
| `IDLE` | Esperando detección de movimiento |
| `ABIERTO` | Tapa abierta, esperando 5 segundos |
| `AVISANDO` | LEDs parpadeando, cerrando en 5 segundos |
| `CERRANDO` | Motor cerrando la tapa |

El uso de `millis()` en lugar de `delay()` permite que Blynk, el PIR y el ultrasonido funcionen de forma simultánea sin bloqueos.

---

## 💡 Relación con el Ciclo SMR

Este proyecto integra conocimientos de diversas asignaturas:

- **Redes Locales:** Protocolos TCP/IP, configuración de redes WiFi y servicios IoT.
- **Montaje y Mantenimiento:** Diseño de circuitos, conexionado con protoboard y gestión de la alimentación.
- **Sistemas Operativos:** Lógica de control mediante máquina de estados y gestión de recursos del microcontrolador.
- **Aplicaciones Web:** Arquitectura cliente-servidor y comunicación con plataformas en la nube.
- **Aplicaciones Ofimáticas:** Documentación técnica y gestión de proyectos.

---

## 👥 Autores

- Patrick Denis Gansca
- Andrei Lucian Iaroi
- Oscar Sacedo Sardinero

Desarrollado en el **IES Nuestra Señora de la Victoria de Lepanto** (2025-2026).
