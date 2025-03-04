#include <Arduino.h>
#include "plant.h"
#include "plant.cpp"
#include "pflanzenliste.h"
#include <SD.h>

void humidity_control(plant *);
unsigned long data_frequency = 500;
int update_time = 1000; // time in ms between updates: how long the arduino waits bewteen loops
int wet_sensor_value = 186;
int dry_sensor_value = 513;

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Set pinmodes
  pinMode(Zimmerpflanze.sensor_pin, INPUT);
  pinMode(Zimmerpflanze.motor_pin, OUTPUT);
  pinMode(Zimmerpflanze.SD_card_pin, OUTPUT);

  // to calibrate the sensor remove the two int in the function
  Zimmerpflanze.calibrate_humidity_sensor(wet_sensor_value,dry_sensor_value);
}

void loop()
{
  Zimmerpflanze.write_to_SDcard(data_frequency);
  humidity_control(&Zimmerpflanze);
  delay(update_time);
  Zimmerpflanze.write_to_pc(data_frequency);
}

void humidity_control(plant *Pflanze)
{
  float h_diff = Pflanze->measure_humidity();
  if (h_diff > 0)
  {
    Pflanze->watering(h_diff, 25);
    // Serial.println("The humidity is ");
    // Serial.println(Pflanze->humidity);
  }
  else
  {
    // Serial.println("The humidity is ");
    // Serial.println(Pflanze->humidity);
  }
}