#include <Arduino.h>
#include "plant.h"
#include "plant.cpp"
#include "pflanzenliste.h"
#include <SD.h>

void humidity_control(plant *);
unsigned long data_frequency = 50000;
int update_time = 1000; // time in ms between updates: how long the arduino waits bewteen loops

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Serial.println("Starting up");
}

void loop()
{
  Zimmerpflanze.write_to_SDcard(data_frequency);
  humidity_control(&Zimmerpflanze);
  delay(update_time);

  Serial.println(millis());
}

void humidity_control(plant *Pflanze)
{
  float h_diff = Pflanze->measure_humidity();
  if (h_diff > 0)
  {
    Pflanze->watering(h_diff, 25);
    Serial.println("The humidity is ");
    Serial.println(Pflanze->humidity);
  }
  else
  {
    Serial.println("The humidity is ");
    Serial.println(Pflanze->humidity);
  }
}