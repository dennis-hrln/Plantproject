#include <Arduino.h>
#include "plant.h"
#include "plant.cpp"
#include "pflanzenliste.h"
#include <SD.h>
#include <screen.h>
#include <LiquidCrystal_I2C.h>
#include <screen.cpp>


LiquidCrystal_I2C lcd(0x27, 16, 2);

screen lcd_screen(&lcd);

void humidity_control(plant *);
void screen_loop(unsigned long, unsigned long, unsigned long);
unsigned long data_frequency = 5000; // time in ms between data writes to the SD card
int update_time = 10000; // time in ms between updates: how long the arduino waits bewteen loops


int wet_sensor_value = 186;
int dry_sensor_value = 513;

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  lcd_screen.innit();
  // Set pinmodes
  pinMode(Zimmerpflanze.sensor_pin, INPUT);
  pinMode(Zimmerpflanze.motor_pin, OUTPUT);
  pinMode(Zimmerpflanze.SD_card_pin, OUTPUT);

  // to calibrate the sensor remove the two int in the function
  Zimmerpflanze.calibrate_humidity_sensor(wet_sensor_value,dry_sensor_value);
}

void loop()
{
  lcd_screen.screen_dimming();
  

  Zimmerpflanze.write_to_SDcard(data_frequency);
  humidity_control(&Zimmerpflanze);
  screen_loop(15000, 10000, 45000);
  delay(update_time);
  Zimmerpflanze.write_to_pc(data_frequency);
  lcd_screen.last_disp_action = millis(); //simiulating button press
}

void humidity_control(plant *Pflanze)
{
  float h_diff = Pflanze->measure_humidity();
  if (h_diff > 0)
  {
    Pflanze->watering(h_diff, 100);
    // Serial.println("The humidity is ");
    // Serial.println(Pflanze->humidity);
  }
  else
  {
    // Serial.println("The humidity is ");
    // Serial.println(Pflanze->humidity);
  }
}

void screen_loop(unsigned long looping_time_home = 15 *1000, unsigned long looping_time_watering = 10 *1000, unsigned long max_page_time = 45*1000)  // time in ms between updates: how long the arduino waits bewteen loops
{
    if ((millis() - lcd_screen.last_disp_action > looping_time_home) && (lcd_screen.disp_status == "home"))
    
    {
      lcd_screen.water_disp();
    }
    else if ((millis() - lcd_screen.last_disp_action > looping_time_watering) && (lcd_screen.disp_status == "water_disp"))
    {
      lcd_screen.home_disp(Zimmerpflanze.planttype, Zimmerpflanze.humidity, Zimmerpflanze.optimal_humidity);
    }
    else if (lcd_screen.disp_status == "startup")
    {
      lcd_screen.home_disp(Zimmerpflanze.planttype, Zimmerpflanze.humidity, Zimmerpflanze.optimal_humidity);
    }
    else if ((millis() - lcd_screen.last_disp_action > max_page_time))
    {
      lcd_screen.home_disp(Zimmerpflanze.planttype, Zimmerpflanze.humidity, Zimmerpflanze.optimal_humidity);
    }


}