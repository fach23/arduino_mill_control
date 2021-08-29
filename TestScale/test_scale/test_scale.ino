#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A3;
const int LOADCELL_SCK_PIN = A2;

HX711 scale;

void setup() {
  Serial.begin(57600);
  Serial.print("power load cell - start");
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
  Serial.print("power load cell - end");

   scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  Serial.print("hello: ");

  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
  
}
