/**
 * Projekt: PIR Bewegungsmelder mit NodeMCU V3 (Erweitert)
 * Beschreibung: Lässt die Onboard-LED 5x kurz blinken, wenn Bewegung erkannt wird.
 */

// Pin-Definitionen
const int PIR_PIN = D1;       // PIR-Sensor ist mit GPIO D1 verbunden
const int LED_PIN = LED_BUILTIN; // Onboard-LED der NodeMCU (ist LOW-aktiv!)

// Variablen für den Sensorstatus
int pirState = LOW;
int val = 0;

void setup() {
  // Seriellen Monitor für Debugging starten
  Serial.begin(115200);
  
  // Pin-Modi festlegen
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // LED initial ausschalten (HIGH schaltet sie beim ESP8266 aus)
  digitalWrite(LED_PIN, HIGH); 
  
  Serial.println("System gestartet. Warte auf PIR-Stabilisierung...");
  delay(10000); // PIR-Sensoren brauchen oft ein paar Sekunden zum Kalibrieren
  Serial.println("PIR bereit.");
}

void loop() {
  val = digitalRead(PIR_PIN); // Sensor auslesen
  
  if (val == HIGH) { // Bewegung erkannt
    if (pirState == LOW) {
      Serial.println("Bewegung erkannt!");
      pirState = HIGH;
      
      // Von 2 auf 5 x kurz blinken geändert
      blinkLED(5, 150); 
    }
  } else {
    if (pirState == HIGH) {
      Serial.println("Bewegung beendet.");
      pirState = LOW;
    }
  }
}

// Funktion zum Blinken der LED
void blinkLED(int mal, int dauer) {
  for (int i = 0; i < mal; i++) {
    digitalWrite(LED_PIN, LOW);  // LED an (LOW-aktiv beim ESP8266)
    delay(dauer);
    digitalWrite(LED_PIN, HIGH); // LED aus
    delay(dauer);
  }
}
