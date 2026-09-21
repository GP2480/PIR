# NodeMCU V3 PIR LED Projekt

### Hardware-Komponenten
- **Controller:** NodeMCU V3 (ESP8266)
- **Sensor:** PIR Bewegungsmelder (z.B. HC-SR501 oder AM312)
- **Aktuator:** Onboard-LED (`LED_BUILTIN`, entspricht GPIO 2 / D4)

### Verkabelung
- **PIR VCC** -> NodeMCU 3V3 (oder Vin, falls der Sensor 5V benötigt)
- **PIR GND** -> NodeMCU GND
- **PIR OUT** -> NodeMCU D1 (GPIO 5)

### Besonderheiten
- Die Onboard-LED des ESP8266 ist **LOW-aktiv**. Das bedeutet: `digitalWrite(LED_PIN, LOW)` schaltet die LED *ein*, `HIGH` schaltet sie *aus*.


