# IBin: Gestión Inteligente de Residuos IoT 🗑️🌐

IBin es un proyecto intermodular desarrollado para el ciclo de **Sistemas Microinformáticos y Redes (SMR)**. Consiste en una papelera inteligente diseñada para mejorar la higiene y optimizar la gestión de residuos mediante el uso de tecnologías de Internet de las Cosas (IoT).

## 🚀 Características Principales

* **Apertura Touchless:** Apertura automática de la tapa mediante un sensor de presencia PIR para evitar el contacto físico.
* **Monitoreo de Nivel:** Medición en tiempo real del llenado mediante un sensor de ultrasonidos.
* **Interfaz Visual LED:** Tira LED NeoPixel que actúa como un semáforo (Verde/Naranja/Rojo) indicando la capacidad disponible.
* **Conectividad Cloud:** Integración con la plataforma **Blynk** para supervisión remota desde dispositivos móviles.
* **Modo Noche:** Configuración remota para desactivar la apertura automática durante horarios específicos.

## 🛠️ Hardware Utilizado

| Componente | Función |
| :--- | :--- |
| **ESP32** | Microcontrolador central con WiFi integrado. |
| **HC-SR04** | Sensor de ultrasonidos para medir el llenado. |
| **HC-SR501 (PIR)** | Sensor infrarrojo para detectar movimiento/presencia. |
| **Servo SG90** | Actuador para el mecanismo de apertura de la tapa. |
| **WS2812B (NeoPixel)** | Tira LED para la interfaz de usuario. |
| **Fuente 5V 3A** | Alimentación estable para evitar caídas de tensión. |

## 📂 Estructura del Proyecto

* `/src`: Código fuente de Arduino (.ino).
* `/docs`: Memoria técnica, esquemas eléctricos y presupuestos.
* `/hardware`: Modelos para impresión 3D (PLA).

## 🔧 Configuración y Montaje

### Conexiones (Pinout ESP32)
* **Sensor PIR:** GPIO 13
* **Ultrasonido:** Trig (GPIO 5), Echo (GPIO 18)
* **Servomotor:** GPIO 12
* **Tira LED:** GPIO 14

### Instalación de Librerías
Es necesario instalar las siguientes librerías en el IDE de Arduino:
1. `ESP32Servo`
2. `Adafruit NeoPixel`
3. `Blynk`

## 💡 Relación con el Ciclo SMR
Este proyecto integra conocimientos de diversas asignaturas:
* **Redes Locales:** Protocolos TCP/IP, configuración de redes WiFi y servicios IoT.
* **Montaje y Mantenimiento:** Diseño de circuitos, soldadura y gestión de energía.
* **Aplicaciones Ofimáticas:** Documentación técnica y gestión de proyectos.

## 👥 Autores
* Patrick Denis Gansca
* Andrei Lucian Iaroi
* Oscar Sacedo Sardinero

---
*Desarrollado en el IES Nuestra Señora de la Victoria de Lepanto (2025-2026).*
