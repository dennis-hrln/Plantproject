#include <Arduino.h>
#include "plant.h"
#include "plant.cpp"
#include "pflanzenliste.h"
#include <SD.h>

void humidity_control(plant*);
const int PIN_SPI_CS = 10;
unsigned long data_frequency = 50000;


void setup() {
    // initialize serial communications at 9600 bps:
    Serial.begin(9600);
    Serial.println("Starting up");

    Zimmerpflanze.write_to_SDcard(PIN_SPI_CS, true);
  }

void loop() {
    humidity_control(&Zimmerpflanze);
    Zimmerpflanze.write_to_SDcard(PIN_SPI_CS, data_frequency);
    delay(100);//00);
    Serial.println(millis());

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