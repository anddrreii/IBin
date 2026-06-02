#define BLYNK_TEMPLATE_ID "TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "IBin"
#define BLYNK_AUTH_TOKEN "AUTH_TOKEN"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Stepper.h>

char ssid[] = "SSID_NAME";
char pass[] = "PASSWD";

// MOTOR
const int pasosPorVuelta = 2048;
Stepper motor(pasosPorVuelta, 13, 14, 12, 27);

// PINES SENSORES Y LEDS
const int PIR          = 26;
const int TRIG         = 5;
const int ECHO         = 18;
const int LED_VERDE    = 17;
const int LED_NARANJA  = 16;
const int LED_ROJO     = 19;

// TIEMPOS
const unsigned long TIEMPO_ABIERTO     = 10000;
const unsigned long TIEMPO_AVISO       = 5000;
const unsigned long INTERVALO_SENSOR   = 1000;
const unsigned long INTERVALO_PARPADEO = 300;

// ESTADO
enum Estado { IDLE, ABIERTO, AVISANDO, CERRANDO };
Estado estadoActual = IDLE;

bool pirPreparado  = false;
int  pirAnterior   = LOW;
float distancia    = 999;
bool modoNoche     = false;   // ← controlado por V2

unsigned long tiempoApertura = 0;
unsigned long tiempoParpadeo = 0;
unsigned long tiempoSensor   = 0;
int  contParpadeo = 0;
bool estadoLED    = false;

// ── Blynk V2 - switch modo noche ──
BLYNK_WRITE(V2) {
  modoNoche = param.asInt();   // 1 = noche activado, 0 = desactivado
  Serial.print("Modo noche: ");
  Serial.println(modoNoche ? "ACTIVADO" : "DESACTIVADO");

  if (modoNoche) {
    apagarLEDs();
    Serial.println("TAPA BLOQUEADA - modo noche");
  } else {
    ledNivel(distancia);
    Serial.println("TAPA DESBLOQUEADA");
  }
}

// ── Motor ──
void bajar() {
  Serial.println("BAJANDO");
  motor.step(-700);
}

void subir() {
  Serial.println("SUBIENDO");
  motor.step(600);
}

// ── Sensor distancia con media de 5 lecturas ──
float medirDistancia() {
  float suma = 0;
  int validas = 0;

  for (int i = 0; i < 5; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duracion = pulseIn(ECHO, HIGH, 15000);

    if (duracion > 0) {
      float d = duracion * 0.0343 / 2.0;
      if (d > 1 && d < 40) {
        suma += d;
        validas++;
      }
    }
    delay(20);
  }

  if (validas == 0) return 999;
  return suma / validas;
}

// ── LEDs ──
void apagarLEDs() {
  digitalWrite(LED_VERDE,   LOW);
  digitalWrite(LED_NARANJA, LOW);
  digitalWrite(LED_ROJO,    LOW);
}

void todasLEDs(bool estado) {
  digitalWrite(LED_VERDE,   estado);
  digitalWrite(LED_NARANJA, estado);
  digitalWrite(LED_ROJO,    estado);
}

void ledNivel(float d) {
  apagarLEDs();
  if      (d > 15) digitalWrite(LED_VERDE,   HIGH);
  else if (d > 5)  digitalWrite(LED_NARANJA, HIGH);
  else             digitalWrite(LED_ROJO,    HIGH);
}

// ── Blynk ──
int nivelPorDistancia(float d) {
  if (d >= 15) return 0;
  if (d <= 5)  return 100;
  int nivel = map(d * 10, 150, 50, 0, 100);
  return constrain(nivel, 0, 100);
}

void enviarBlynk(float d) {
  Blynk.virtualWrite(V0, nivelPorDistancia(d));           // nivel en %
  Blynk.virtualWrite(V1, estadoActual == IDLE ? 0 : 1);  // tapa 0=cerrada 1=abierta
}

// ── Setup ──
void setup() {
  Serial.begin(115200);

  pinMode(PIR,         INPUT);
  pinMode(TRIG,        OUTPUT);
  pinMode(ECHO,        INPUT);
  pinMode(LED_VERDE,   OUTPUT);
  pinMode(LED_NARANJA, OUTPUT);
  pinMode(LED_ROJO,    OUTPUT);

  motor.setSpeed(5);
  apagarLEDs();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Calibrando PIR...");
  unsigned long t = millis();
  while (millis() - t < 5000) Blynk.run();
  Serial.println("Sistema listo");

  distancia = medirDistancia();
  ledNivel(distancia);
  enviarBlynk(distancia);
}

// ── Loop ──
void loop() {
  Blynk.run();

  unsigned long ahora = millis();

  // Medir distancia cada 1 segundo
  if (ahora - tiempoSensor >= INTERVALO_SENSOR) {
    tiempoSensor = ahora;
    distancia = medirDistancia();
    Serial.print("Distancia: "); Serial.print(distancia); Serial.println(" cm");
    if (estadoActual == IDLE && !modoNoche) {
      ledNivel(distancia);
      enviarBlynk(distancia);
    }
  }

  // Leer PIR
  int pirActual = digitalRead(PIR);
  if (pirActual == LOW) pirPreparado = true;

  // Detectar movimiento
  if (estadoActual == IDLE &&
      pirPreparado  &&
      pirActual == HIGH &&
      pirAnterior == LOW) {

    if (modoNoche) {
      // Modo noche activo — tapa bloqueada
      Serial.println("MODO NOCHE - tapa bloqueada");
      apagarLEDs();

    } else if (distancia <= 5) {
      // Papelera llena — tapa bloqueada
      Serial.println("PAPELERA LLENA - bloqueada");
      apagarLEDs();
      digitalWrite(LED_ROJO, HIGH);
      enviarBlynk(distancia);

    } else {
      // Abrir normal
      Serial.println("ABRIENDO");
      todasLEDs(true);
      bajar();
      estadoActual   = ABIERTO;
      tiempoApertura = millis();
      pirPreparado   = false;
      apagarLEDs();
      Blynk.virtualWrite(V1, 1);
    }
  }

  pirAnterior = pirActual;

  // A los 5 segundos aviso
  if (estadoActual == ABIERTO &&
      millis() - tiempoApertura >= TIEMPO_AVISO) {
    estadoActual   = AVISANDO;
    contParpadeo   = 0;
    estadoLED      = false;
    tiempoParpadeo = millis();
    Serial.println("AVISO - cerrando en 5 segundos");
  }

  // Parpadeo aviso
  if (estadoActual == AVISANDO) {
    if (millis() - tiempoParpadeo >= INTERVALO_PARPADEO) {
      tiempoParpadeo = millis();
      estadoLED = !estadoLED;
      todasLEDs(estadoLED);
      contParpadeo++;
    }
    if (millis() - tiempoApertura >= TIEMPO_ABIERTO) {
      estadoActual = CERRANDO;
    }
  }

  // Cerrar
  if (estadoActual == CERRANDO) {
    Serial.println("CERRANDO");
    apagarLEDs();
    subir();
    distancia = medirDistancia();
    if (!modoNoche) ledNivel(distancia);
    enviarBlynk(distancia);
    Blynk.virtualWrite(V1, 0);
    estadoActual = IDLE;
    pirPreparado = false;
    Serial.println("CERRADA - standby");
  }
}
