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

void screen_loop(unsigned long looping_time_home = 15 * 1000, unsigned long looping_time_watering = 10 * 1000, unsigned long max_page_time = 45 * 1000) // time in ms between updates: how long the arduino waits bewteen loops
{
  if ((millis() - lcd_screen.last_disp_change > looping_time_home) && (lcd_screen.disp_status == "home"))

  {
    lcd_screen.water_disp(Zimmerpflanze.last_watered);
  }
  else if ((millis() - lcd_screen.last_disp_change > looping_time_watering) && (lcd_screen.disp_status == "water_disp"))
  {
    lcd_screen.home_disp(Zimmerpflanze.planttype, Zimmerpflanze.humidity, Zimmerpflanze.optimal_humidity);
  }
  else if (lcd_screen.disp_status == "startup")
  {
    lcd_screen.home_disp(Zimmerpflanze.planttype, Zimmerpflanze.humidity, Zimmerpflanze.optimal_humidity);
  }
  else if ((millis() - lcd_screen.last_disp_change > max_page_time))
  {
    lcd_screen.home_disp(Zimmerpflanze.planttype, Zimmerpflanze.humidity, Zimmerpflanze.optimal_humidity);
  }
}