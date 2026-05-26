/*
 * PROYECTO: IBin Smart Bin - TFG SMR
 * FUNCIONALIDAD: Apertura PIR, Nivel por Ultrasonido, Semáforo LED y App Blynk.
 */

#define BLYNK_TEMPLATE_ID "TMPL5apTqd71X"
#define BLYNK_TEMPLATE_NAME "IBInTFG"
#define BLYNK_AUTH_TOKEN "kenBDqkYKFrQWKsPGHyzbpPgH1HPtdEv"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

// Credenciales
char auth[] = "kenBDqkYKFrQWKsPGHyzbpPgH1HPtdEv";
char ssid[] = "andrei"; 
char pass[] = "andrei123";

// Configuración Pins
#define PIN_LEDS 14
#define NUM_LEDS 10
#define PIN_PIR  27
#define PIN_SERVO 13
#define PIN_TRIG 5
#define PIN_ECHO 18

// Objetos
Adafruit_NeoPixel tira(NUM_LEDS, PIN_LEDS, NEO_GRB + NEO_KHZ800);
Servo miServo;
BlynkTimer timer;

// Variables Globales
bool tapaAbierta = false;
bool bloqueoBlynk = false; // Controlado por V3
float distanciaActual = 0;
int porcentajeLlenado = 0;
const int ALTURA_PAPELERA = 25; // cm (ajusta según tu bote)

// Función: Colores del Semáforo (4V, 3N, 3R) - INVERTIDA
void mostrarSemaforo() {
  tira.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t color = tira.Color(0, 0, 0); // Color temporal apagado

    // Determinamos qué color le toca al índice 'i'
    if (distanciaActual > 18) { // Estado VACÍO
      if (i < 4) color = tira.Color(0, 255, 0); 
    } else if (distanciaActual > 10) { // Estado MEDIO
      if (i < 4) color = tira.Color(0, 255, 0);
      if (i >= 4 && i < 7) color = tira.Color(255, 100, 0);
    } else { // Estado LLENO
      if (i < 4) color = tira.Color(0, 255, 0);
      if (i >= 4 && i < 7) color = tira.Color(255, 100, 0);
      if (i >= 7) color = tira.Color(255, 0, 0);
    }
    
    // Aplicamos la inversión: el índice 0 se dibuja en el LED 9
    tira.setPixelColor(9 - i, color); 
  }
  tira.show();
}

// Función: Parpadeo de Alerta (Últimos 3 segundos)
void parpadearAlerta() {
  for (int j = 0; j < 3; j++) { 
    tira.clear();
    tira.show();
    delay(500);
    mostrarSemaforo();
    delay(500);
  }
}

// Medición de nivel y envío a Blynk
void medirNivel() {
  digitalWrite(PIN_TRIG, LOW); delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long duracion = pulseIn(PIN_ECHO, HIGH);
  distanciaActual = duracion * 0.034 / 2;

  porcentajeLlenado = map(constrain(distanciaActual, 5, ALTURA_PAPELERA), 5, ALTURA_PAPELERA, 100, 0);
  
  Blynk.virtualWrite(V1, porcentajeLlenado); 
  if (!tapaAbierta) mostrarSemaforo();
}

// Lógica de apertura
void gestionarApertura() {
  if (digitalRead(PIN_PIR) == HIGH && !tapaAbierta && !bloqueoBlynk) {
    tapaAbierta = true;
    Blynk.virtualWrite(V2, 1); 
    
    miServo.write(55); // Posición 55 para recorrido de 35 grados
    delay(3000);       
    
    parpadearAlerta(); 
    
    miServo.write(90); // Vuelve a reposo (cerrado)
    tapaAbierta = false;
    Blynk.virtualWrite(V2, 0);
  }
}

BLYNK_WRITE(V3) {
  bloqueoBlynk = param.asInt(); 
}

void setup() {
  Serial.begin(115200);
  tira.begin();
  tira.setBrightness(150); 
  
  miServo.attach(PIN_SERVO);
  miServo.write(90); 
  
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, medirNivel); 
}

void loop() {
  Blynk.run();
  timer.run();
  gestionarApertura();
}