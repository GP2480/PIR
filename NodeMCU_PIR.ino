
/**
 * Projekt: PIR Bewegungsmelder mit NodeMCU V3 (Deaktivierbar)
 * Beschreibung: Lässt die Onboard-LED 2x kurz blinken, wenn Bewegung erkannt wird.
 *               Über einen Taster an D0 (entprellt) kann der PIR-Sensor ein-/ausgeschaltet werden.
 */

// Pin-Definitionen
const int PIR_PIN = D1;       // PIR-Sensor ist mit GPIO D1 verbunden
const int BUTTON_PIN = D0;    // Taster ist mit GPIO D0 verbunden
const int LED_PIN = LED_BUILTIN; // Onboard-LED der NodeMCU (ist LOW-aktiv!)

// Variablen für den Sensorstatus
int pirState = LOW;
int val = 0;

// Variablen für den Taster und das Entprellen (Debounce)
bool pirEnabled = true;       // Status, ob der PIR aktiv ist
int lastButtonState = HIGH;   // Vorheriger Zustand des Tasters (Pull-Up -> Standard HIGH)
unsigned long lastDebounceTime = 0;  // Die Zeit des letzten Flankenwechsels
const unsigned long debounceDelay = 50; // Entprell-Zeit in Millisekunden

void setup() {
  // Seriellen Monitor für Debugging starten
  Serial.begin(115200);
  
  // Pin-Modi festlegen
  pinMode(PIR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Internen Pull-Up-Widerstand aktivieren
  pinMode(LED_PIN, OUTPUT);
  
  // LED initial ausschalten (HIGH schaltet sie beim ESP8266 aus)
  digitalWrite(LED_PIN, HIGH); 
  
  Serial.println("System gestartet. Warte auf PIR-Stabilisierung...");
  delay(10000); // PIR-Sensoren brauchen oft ein paar Sekunden zum Kalibrieren
  Serial.println("PIR bereit. Status: AKTIV (Taster an D0 zum Deaktivieren)");
}

void loop() {
  // 1. TASTER AUSLESEN & ENTPRELLEN
  int reading = digitalRead(BUTTON_PIN);

  // Wenn sich der Schalterzustand geändert hat (durch Rauschen oder Drücken)
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Zeit zurücksetzen
  }

  // Wenn die Zeitspanne größer als das Entprell-Intervall ist
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Wenn der Taster gedrückt wurde (LOW bei INPUT_PULLUP)
    if (reading == LOW && pirEnabled == true) {
      pirEnabled = false;
      Serial.println("--> PIR DEAKTIVIERT (Bewegungen werden ignoriert)");
      
      // Visuelles Feedback: LED leuchtet 1 Sekunde lang als "Aus"-Signal
      digitalWrite(LED_PIN, LOW);
      delay(1000);
      digitalWrite(LED_PIN, HIGH);
      
      delay(200); // Kurze Pause, um Mehrfachauslösung beim Loslassen zu verhindern
    } 
    else if (reading == LOW && pirEnabled == false) {
      pirEnabled = true;
      Serial.println("--> PIR AKTIVIERT (System scharfgestellt)");
      
      // Visuelles Feedback: LED blitzt 3x ganz schnell als "An"-Signal
      blinkLED(3, 50);
      
      delay(200);
    }
  }
  
  // Aktuellen Zustand für den nächsten Durchlauf speichern
  lastButtonState = reading;

  // 2. PIR SENSOR ABFRAGEN (Nur wenn aktiv geschaltet)
  if (pirEnabled) {
    val = digitalRead(PIR_PIN); // Sensor auslesen
    
    if (val == HIGH) { // Bewegung erkannt
      if (pirState == LOW) {
        Serial.println("Bewegung erkannt!");
        pirState = HIGH;
        blinkLED(2, 150); // Standardmäßig 2x blinken aus der Main-Version
      }
    } else {
      if (pirState == HIGH) {
        Serial.println("Bewegung beendet.");
        pirState = LOW;
      }
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
