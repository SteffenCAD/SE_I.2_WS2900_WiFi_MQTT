#include <Arduino.h>
#include <SoftwareSerial.h>

const int rxPin = D2;  // RX-Pin
const int txPin = D3;  // TX-Pin

SoftwareSerial mySerial(rxPin, txPin);

const int bufferSize = 265;
byte buffer[bufferSize];
unsigned long previousMillis = 0;
const int interval = 100;  // Millisekunden

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void printHexBytes(byte* data, int length) {
  for (int i = 0; i < length; i++) {
    if (data[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Amount of received Bytes: ");
  Serial.println(length);
}

void loop() {
  // Überprüfe, ob Daten auf der UART-Schnittstelle verfügbar sind
  while (mySerial.available() > 0) {
    // Lese Daten in den Puffer
    int bytesRead = mySerial.readBytes(buffer, bufferSize);

    // Setze den Timer für die nächste Ausgabe zurück
    previousMillis = millis() + interval;

    // Füge hier nach Bedarf weitere Verarbeitungsschritte für die empfangenen Daten ein

    // Gib Daten aus, wenn der Puffer voll ist oder 100 Millisekunden vergangen sind
    if (bytesRead > 0 || millis() - previousMillis >= interval) {
      Serial.print("Received Hex Bytes: ");
      printHexBytes(buffer, bytesRead);
      Serial.println();
      // Leere den Puffer
      memset(buffer, 0, sizeof(buffer));
    }
  }
}
