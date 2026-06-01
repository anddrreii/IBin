# IBin: Gestión Inteligente de Residuos IoT 🗑️🌐

IBin es un proyecto intermodular desarrollado para el ciclo de Sistemas Microinformáticos y Redes (SMR). Consiste en una papelera inteligente diseñada para mejorar la higiene y optimizar la gestión de residuos mediante el uso de tecnologías de Internet de las Cosas (IoT).

---

## 🚀 Características Principales

- **Apertura Touchless:** Apertura automática de la tapa mediante un sensor de presencia PIR para evitar el contacto físico.
- **Monitoreo de Nivel:** Medición en tiempo real del llenado mediante un sensor de ultrasonidos.
- **Interfaz Visual LED:** Tres diodos LED discretos (verde, naranja y rojo) con resistencias de 330 Ω que actúan como semáforo indicando la capacidad disponible.
- **Conectividad Cloud:** Integración con la plataforma Blynk para supervisión remota desde dispositivos móviles.
- **Modo Noche:** Configuración remota para desactivar la apertura automática durante horarios específicos.

---

## 🛠️ Hardware Utilizado

| Componente | Función |
|---|---|
| ESP32 DevKit V1 | Microcontrolador central con WiFi integrado. |
| HC-SR04 | Sensor de ultrasonidos para medir el nivel de llenado. |
| HC-SR501 (PIR) | Sensor infrarrojo para detectar movimiento y presencia. |
| Motor 28BYJ-48 + Driver ULN2003 | Actuador para el mecanismo de apertura de la tapa mediante hilo de pesca. |
| LEDs discretos 5 mm (x3) + resistencias 330 Ω | Interfaz visual tipo semáforo (verde, naranja, rojo). |
| Protoboard | Distribución central de alimentación para todos los componentes. |
| Fuente 5V / 2A | Alimentación estable del sistema. |

---

## ⚙️ Mecanismo de Apertura

El motor paso a paso 28BYJ-48 lleva unido un soporte en su eje. A ese soporte se ancla un hilo de pesca de alta resistencia, cuyo otro extremo está fijado a un tornillo instalado en la tapa de la papelera.

- Cuando el motor gira en **sentido horario**, el hilo se enrolla en el soporte y tira de la tapa hacia arriba, **abriéndola**.
- Cuando el motor gira en **sentido antihorario**, el hilo se desenrolla y la tapa desciende por su propio peso, **cerrándola**.

---

## 📂 Estructura del Proyecto

```
/src        → Código fuente de Arduino (.ino)
/docs       → Memoria técnica, esquemas eléctricos y presupuestos
/hardware   → Modelos para impresión 3D (PLA)
```

---

## 🔧 Configuración y Montaje

### Conexiones (Pinout ESP32)

| Componente | Pin(es) ESP32 |
|---|---|
| Sensor PIR HC-SR501 | GPIO 27 |
| Ultrasonido TRIG | GPIO 5 |
| Ultrasonido ECHO | GPIO 18 |
| Motor ULN2003 IN1 | GPIO 13 |
| Motor ULN2003 IN2 | GPIO 12 |
| Motor ULN2003 IN3 | GPIO 26 |
| Motor ULN2003 IN4 | GPIO 25 |
| LED verde (+ 330 Ω) | GPIO 14 |
| LED naranja (+ 330 Ω) | GPIO 25 |
| LED rojo (+ 330 Ω) | GPIO 26 |
| VCC (todos) | VIN / 5V (protoboard) |
| GND (todos) | GND (protoboard) |

### Instalación de Librerías

Es necesario instalar las siguientes librerías en el IDE de Arduino:

- `Stepper` (incluida en el IDE de Arduino, no requiere instalación adicional)
- `Blynk`

> **Nota:** Las librerías `ESP32Servo` y `Adafruit NeoPixel` ya no son necesarias en esta versión del proyecto.

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
