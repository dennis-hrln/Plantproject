#include <Arduino.h>
#include <screen.h>
#include <LiquidCrystal_I2C.h>
//in main LiquidCrystal_I2C lcd(0x27, 16, 2); --> pointer in class definition

screen::screen(LiquidCrystal_I2C* LCD)
{
    this->lcd = LCD;
};

void screen::innit()
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Instagram");
    lcd.setCursor(0, 1);
    lcd.print("dennis_hrln");
    delay(2000);
    lcd.clear();
};
void screen::screen_dimming()
{
    if (millis() - this->last_disp_change > this->stay_awake_time)
    {
        lcd.noBacklight();
    }
};

void screen::home_disp(String plantname, int humidity, int optimal_humidity)
{
    lcd.setCursor(0, 0);
    lcd.print(plantname);
    lcd.setCursor(0, 1);
    lcd.print("Hum. " + String(humidity) + "% /" + String(optimal_humidity) + "%");
};

