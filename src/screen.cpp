#include <Arduino.h>
#include <screen.h>
#include <LiquidCrystal_I2C.h>
//in main LiquidCrystal_I2C lcd(0x27, 16, 2); --> pointer in class definition

screen::screen(LiquidCrystal_I2C* LCD)
{
    this->lcd = LCD;
    this->disp_status = STARTUP;
};

void screen::innit()
{
    lcd->init();
    lcd->backlight();
    lcd->setCursor(0, 0);
    lcd->print("Instagram");
    lcd->setCursor(0, 1);
    lcd->print(F("3dEngineeringDE"));
    this->disp_status = STARTUP;
    Serial.println(F("lcd is starting up..."));
    Serial.println(this->disp_status);
    lcd->createChar(0, select_char);
    delay(2000);
    // lcd->createChar(1, left_arrow);
    // lcd->createChar(2, right_arrow);
    // lcd->createChar(3, down_arrow);
    // lcd->createChar(4, up_arrow);
    
    

};
void screen::screen_dimming(const char* plantname, int humidity, int optimal_humidity)
{
    if (millis() - this->last_disp_action > this->stay_awake_time)
    {
        lcd->noBacklight();
        this->lit = false;
        home_disp(plantname, humidity, optimal_humidity);
    }
};

void screen::home_disp(const char* plantname, int humidity, int optimal_humidity)
{
    this->disp_status = HOME;
    lcd->clear();
    if (this->lit == true)
    {
        lcd->backlight();
    }
    lcd->setCursor(0, 0);
    lcd->print(plantname);
    lcd->setCursor(0, 1);
    lcd->print("Hum. ");
    lcd->print(humidity);
    lcd->print("% / ");
    lcd->print(optimal_humidity);
    lcd->print("%");
    last_disp_change = millis();
};

void screen::water_disp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute)
{
    //logic for last watered into months days hours mins seconds
    this->disp_status = WATER_DISP;
    lcd->clear();
    if (this->lit == true)
    {
        lcd->backlight();
    }
    lcd->setCursor(0, 0);
    lcd->print("Last watered:");
    lcd->setCursor(0, 1);
    lcd->print(day);
    lcd->print(F("."));
    lcd->print(month);
    lcd->print(F("."));
    lcd->print(year);
    lcd->print(F(" "));
    lcd->print(hour);
    lcd->print(F(":"));
    lcd->print(minute);
    //todo replace milis with time from time module
    last_disp_change = millis();
};

void screen::calibration_disp()
{
    lcd->clear();
    if (this->lit == true)
    {
        lcd->backlight();
    }
    lcd->setCursor(0, 0);
    lcd->print("calibrate dry");
    lcd->setCursor(0, 1);
    lcd->print("calibrate wet");
    if (this->disp_status == screen::WET_CALIBRATION)
    {
        lcd->setCursor(15, 1);
        lcd->write(0);
    }
    else
    {
        lcd->setCursor(15, 0);
        lcd->write(0);
        this->disp_status = screen::DRY_CALIBRATION;        
    }
    last_disp_change = millis();
};

void screen::calibrated_value_disp(bool is_dry_calibration, int new_value)
{
    lcd->clear();
    if (this->lit == true)
    {
        lcd->backlight();
    }
    lcd->setCursor(0, 0);
    if (is_dry_calibration == true)
    {
        lcd->print("New Dry Value: ");
    }
    else
    {
        lcd->print("New Wet Value: ");
    }
    lcd->setCursor(0, 1);
    lcd->print(new_value);

    last_disp_change = millis();
};

void screen::date_disp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second)
{
    lcd->clear();
    if (this->lit == true)
    {
        lcd->backlight();
    }
    //write date and time
    lcd->setCursor(0, 0);
    lcd->print(year);
    lcd->print("/");
    lcd->print(month);
    lcd->print("/");
    lcd->print(day);
    lcd->setCursor(0, 1);
    lcd->print(hour);
    lcd->print(F(":"));
    lcd->print(minute);
    lcd->print(F(":"));
    lcd->print(second);

    // show what would be edited when pressing the edit button
    if (this->disp_status == DATE_DISP)
    {
        lcd->setCursor(15,0);
        lcd->write(0);
        switch(this->disp_status)
        {
            case EDIT_YEAR:
                lcd->setCursor(1, 0);
                lcd->blink();
                break;
            case EDIT_MONTH:
                lcd->setCursor(3, 0);
                lcd->blink();
                break;
            case EDIT_DAY:
                lcd->setCursor(6, 0);
                lcd->blink();
                break;
            default:
                break;
        }
    }
    else
    {
        this->disp_status = TIME_DISP;
        lcd->setCursor(15, 1);
        lcd->write(0);
        switch(this->disp_status)
        {
            case EDIT_HOUR:
                lcd->setCursor(9, 0);
                lcd->blink();
                break;
            case EDIT_MINUTE:
                lcd->setCursor(12, 0);
                lcd->blink();
                break;
            case EDIT_SECOND:
                lcd->setCursor(15, 0);
                lcd->blink();
                break;
            default:
                break;
        }
    }

    last_disp_change = millis();
    

};

void screen::update_screen(const char* plantname, int humidity, int optimal_humidity,
unsigned int watered_year, unsigned int  watered_month, unsigned int  watered_day, unsigned int  watered_hour, unsigned int  watered_minute,
unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minutes, unsigned int seconds)
{
    if (this->disp_status == HOME)
    {
        this->home_disp(plantname, humidity, optimal_humidity);
    }
    else if (this->disp_status == WATER_DISP)
    {
        this->water_disp( watered_year,  watered_month,  watered_day,  watered_hour,  watered_minute);
    }
    else if (this->disp_status == DATE_DISP || this->disp_status == TIME_DISP)
    {
        this->date_disp(year, month, day, hour, minutes, seconds);
    }
    else if (this->disp_status == STARTUP)
    {
        this->home_disp(plantname, humidity, optimal_humidity);
    }
}