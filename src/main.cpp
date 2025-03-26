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
void calibration();
void screen_loop(unsigned long, unsigned long, unsigned long);
void next_button();
void select_button();
unsigned long data_frequency = 5000; // time in ms between data writes to the SD card
volatile bool next_button_pressed = false;
volatile bool select_button_pressed = false;
const byte nextButtonPIN = 2;
const byte selectButtonPIN = 3;
bool dry_calibrated = true;
bool wet_calibrated = true;

void check_next_button()
{
  next_button_pressed = true;
  Serial.println(F("next button pressed"));
  
}
void check_select_button()
{
  Serial.println(F("select button pressed"));
  select_button_pressed = true;
}

int wet_sensor_value = 186;
int dry_sensor_value = 513;

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  lcd_screen.innit();
  // Set pinmodes
  pinMode(Stirps.sensor_pin, INPUT);
  pinMode(Stirps.motor_pin, OUTPUT);
  pinMode(Stirps.SD_card_pin, OUTPUT);
  pinMode(nextButtonPIN, INPUT_PULLUP);
  pinMode(selectButtonPIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(nextButtonPIN), check_next_button, RISING);
  attachInterrupt(digitalPinToInterrupt(selectButtonPIN), check_select_button, RISING);
  // to calibrate the sensor remove the two int in the function
  Stirps.calibrate_humidity_sensor(wet_sensor_value, dry_sensor_value);
}

void loop()
{
  lcd_screen.screen_dimming();

  Stirps.write_to_SDcard(data_frequency);
  humidity_control(&Stirps);
  // screen_loop(15000, 10000, 45000);
  Stirps.write_to_pc(data_frequency);
  calibration();
  if (next_button_pressed)
  {
    next_button();
  }
  if (select_button_pressed)
  {
    select_button();
  }
  lcd_screen.update_screen(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity, Stirps.last_watered);;
  // delay(1000);
}

void humidity_control(plant *Pflanze)
{
  float h_diff = Pflanze->measure_humidity();
  if (h_diff > 0)
  {
    Pflanze->watering(h_diff, 100);
  }
}



void calibration()
{
  if (dry_calibrated == false)
  {
    lcd_screen.lcd->clear();
    lcd_screen.lcd->setCursor(0, 0);
    lcd_screen.lcd->print("Dry calibration");
    lcd_screen.lcd->setCursor(0, 1);
    lcd_screen.lcd->print("wait 30s");
    Stirps.humidity_sensor_dry_calibration();
    dry_calibrated = true;
    lcd_screen.lcd->clear();
    lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
  }
  if (wet_calibrated == false)
  {
    lcd_screen.lcd->clear();
    lcd_screen.lcd->setCursor(0, 0);
    lcd_screen.lcd->print("Wet calibration");
    lcd_screen.lcd->setCursor(0, 1);
    lcd_screen.lcd->print("wait 30s");
    Stirps.humidity_sensor_wet_calibration();
    wet_calibrated = true;
    lcd_screen.lcd->clear();
    lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
  }
}

void next_button()
{
  if (digitalRead(nextButtonPIN) == HIGH)
  {
    lcd_screen.last_disp_action = millis();

      lcd_screen.lcd->backlight();
      lcd_screen.lit = true;

    if (lcd_screen.disp_status == "home")
    {
      lcd_screen.water_disp(Stirps.last_watered);
    }
    else if (lcd_screen.disp_status == "water_disp")
    {
      lcd_screen.calibration_disp();
    }
    else if (lcd_screen.disp_status == "dry_calibration")
    {
      lcd_screen.disp_status = "wet_calibration";
      lcd_screen.calibration_disp();
    }
    else if (lcd_screen.disp_status == "wet_calibration")
    {
      lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
    }
    else
    {
      lcd_screen.lcd->clear();
      lcd_screen.lcd->setCursor(0, 0);
      lcd_screen.lcd->print("startup");
      lcd_screen.disp_status = "home";
      delay(2000);
      lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
    }
  }
  next_button_pressed = false;
  delay(100);
}

void select_button()
{
  if (digitalRead(selectButtonPIN) == HIGH)
  {
    lcd_screen.last_disp_action = millis();

      lcd_screen.lcd->backlight();
      lcd_screen.lit = true;
    

    if (lcd_screen.disp_status == "dry_calibration")
    {
      dry_calibrated = false;
    }
    else if (lcd_screen.disp_status == "wet_calibration")
    {
      wet_calibrated = false;
    }
  }
  select_button_pressed = false;
  delay(100);
}