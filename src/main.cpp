#include <Arduino.h>
#include "plant.h"
#include "plant.cpp"
#include "pflanzenliste.h"

void humidity_control(plant*);

void setup() {
    // initialize serial communications at 9600 bps:
    Serial.begin(9600);
    Serial.println("Starting up");
  }

void loop() {
    humidity_control(&Zimmerpflanze);

}


void humidity_control(plant* Pflanze){
  float h_diff = Pflanze->measure_humidity();
      if (h_diff > 0)
      {
          Pflanze->watering(h_diff,25);
          Serial.println("The humidity is ");
          Serial.println(Pflanze->humidity);

      }
      else
      {
        Serial.println("The humidity is ");
        Serial.println(Pflanze->humidity);
          delay(60000);
      }
}