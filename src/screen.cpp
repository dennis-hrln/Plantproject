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

void screen::home_disp(const char *plantname, int humidity, int optimal_humidity)
{
    this->disp_status = HOME;
    bool refresh = check_for_change();
    if (this->lit == true)
    {
        lcd->backlight();
    }

    if (refresh == true)
    {

        lcd->setCursor(0, 0);
        lcd->print(plantname);
        lcd->setCursor(0, 1);
        lcd->print("Hum. ");

        lcd->setCursor(8, 1);
        lcd->print("% / ");
        lcd->print(optimal_humidity);
        lcd->print("%");
    }

    if ((prev_humidity != humidity && ((millis() - hum_last_refresh) > hum_refresh_rate)) || refresh == true)
    {
        lcd->setCursor(5, 1);
        if (humidity < 0)
        {
            if (avoid_negative_humidity == true)
            {
                lcd->print(F(" -"));
                humidity = 0;
            }
        }
        else if (humidity < 10)
        {
            lcd->print(F("  "));
        }
        else if (humidity > 99)
        {}
        else if (humidity > 9)
        {
            lcd->print(F(" "));
        }
        
        lcd->print(humidity);
        hum_last_refresh = millis();
    }

    prev_humidity = humidity;
    last_disp_change = millis();
};

void screen::water_disp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute)
//show the last time the plant was watered
{
    //logic for last watered into months days hours mins seconds
    check_for_change();
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
    if (year == 0){
        lcd->print(F("."));
    }
    else{
        lcd->print(F(".20"));
    }
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
    bool refresh = check_for_change();
    if (this->lit == true)
    {
        lcd->backlight();
    }
    if (refresh == true)
        {

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
        }
    this->prev_disp = this->disp_status;
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
    check_for_change();
    if (this->lit == true)
    {
        lcd->backlight();
    }
    //write date and time
    lcd->setCursor(0, 0);
    lcd->print(F("20"));
    if (year < 10)
    {
        lcd->print(F("0"));
    }
    lcd->print(year);
    lcd->print("/");
    if (month < 10)
    {
        lcd->print(F("0"));
    }
    lcd->print(month);
    lcd->print("/");
    if (day < 10)
    {
        lcd->print(F("0"));
    }
    lcd->print(day);
    lcd->setCursor(0, 1);
    if (hour < 10)
    {
        lcd->print(F("0"));
    }
    lcd->print(hour);
    lcd->print(F(":"));
    if (minute < 10)
    {
        lcd->print(F("0"));
    }
    lcd->print(minute);
    lcd->print(F(":"));
    if (second < 10)
    {
        lcd->print(F("0"));
    }
    lcd->print(second);

    // show what would be edited when pressing the edit button
    if (this->disp_status == DATE_DISP)
    {
        lcd->setCursor(15,0);
        lcd->write(0);
        switch(this->edit_time_stat)
        {
            case EDIT_YEAR:
                lcd->setCursor(3, 0);
                if (this->blinking == false)
                {
                    lcd->blink();
                    this->blinking = true;
                }
                break;
            case EDIT_MONTH:
                lcd->setCursor(6, 0);
                if (this->blinking == false)
                {
                    lcd->blink();
                    this->blinking = true;
                }
                break;
            case EDIT_DAY:
                lcd->setCursor(9, 0);
                if (this->blinking == false)
                {
                    lcd->blink();
                    this->blinking = true;
                }
                break;
            default:
            lcd->noBlink();
            this->blinking = false;
                break;
        }
    }
    else
    {
        this->disp_status = TIME_DISP;
        lcd->setCursor(15, 1);
        lcd->write(0);
        switch(this->edit_time_stat)
        {
            case EDIT_HOUR:
                lcd->setCursor(1, 1);
                if (this->blinking == false)
                {
                    lcd->blink();
                    this->blinking = true;
                }
                break;
            case EDIT_MINUTE:
                lcd->setCursor(4, 1);
                if (this->blinking == false)
                {
                    lcd->blink();
                    this->blinking = true;
                }
                break;
            case EDIT_SECOND:
                lcd->setCursor(7, 1);
                if (this->blinking == false)
                {
                    lcd->blink();
                    this->blinking = true;
                }
                break;
            default:
            lcd->noBlink();
            this->blinking = false;
                break;
        }
    }
    last_disp_change = millis();
    

};

/*check if the display has changed and clear the display if it has - retuns true if the display has changed
this is used to prevent flickering of the display when the display is updated 
does not work for display with changing content (like date and time)*/
bool screen::check_for_change()
{
    if (this->prev_disp != this->disp_status)
    {
        lcd->noBlink();
        lcd->noCursor();
        lcd->clear();
        this->prev_disp = this->disp_status;
        return true; //display has changed
    }
    else
    {
        return false; //display has not changed
    }
};

void screen::update_screen(const char* plantname, int humidity, int optimal_humidity,
unsigned int watered_year, unsigned int  watered_month, unsigned int  watered_day, unsigned int  watered_hour, unsigned int  watered_minute,
unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minutes, unsigned int seconds)
{
    switch (this->disp_status)
    {
        case HOME:
            this->home_disp(plantname, humidity, optimal_humidity);
            break;
        case WATER_DISP:
            this->water_disp(watered_year, watered_month, watered_day, watered_hour, watered_minute);
            break;
        case DRY_CALIBRATION:
            this->calibration_disp();
            break;
        case WET_CALIBRATION:
            this->calibration_disp();
            break;
        case DATE_DISP:
            this->date_disp(year, month, day, hour, minutes, seconds);
            break;
        case TIME_DISP:
            this->date_disp(year, month, day, hour, minutes, seconds);
            break;
        case STARTUP:
                lcd->clear();
				lcd->setCursor(0, 0);
				lcd->print("loading...");
				delay(2000);
                this->disp_status = HOME;
            break;
        default:
            break;
    }
}