#include <Arduino.h>
#include "plant.h"
#include "plant.cpp"
#include "pflanzenliste.h"


void setup() {
    Serial.begin(9600);
    Serial.println("Start calibration");
}
void loop() {
    Zimmerpflanze.calibrate_humidity_sensor();
    Serial.println("Calibration done");
    delay(10000);
}