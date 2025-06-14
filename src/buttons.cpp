

#include "buttons.h"
extern Ds1302 rtc; // or Ds1302 rtc, depending on your RTC type
// extern RTC_DS3231 rtc; // or Ds1302 rtc, depending on your RTC type
extern bool rtc_available;
extern bool dry_calibrated;
extern bool wet_calibrated;
extern volatile bool next_button_pressed;
extern volatile bool select_button_pressed;
extern screen lcd_screen;


void select_button(plant *Pflanze, int humidity_sensor_pin)
{
	lcd_screen.last_disp_action = millis();
    lcd_screen.lit = true;
	select_button_pressed = false;

	if (lcd_screen.lit == true)
	{
        switch(lcd_screen.disp_status)
        {
            case screen::HOME:
                lcd_screen.lit = false;
                // Startup case does not need to be handled here - only here for clarity
            break;
            case screen::STARTUP:
                break;
            case screen::WATER_DISP:
                reset_time(&rtc);
                break;
            case screen::DATE_DISP:
                switch(lcd_screen.edit_time_stat)
                {
                    case screen::NO_EDIT:
                        lcd_screen.edit_time_stat = screen::EDIT_YEAR;
                        break;
                    case screen::EDIT_YEAR:
                        Serial.print("adjust year");
                        adjust_year(&rtc);
                        break;
                    case screen::EDIT_MONTH:
                        adjust_month(&rtc);
                        break;
                    case screen::EDIT_DAY:
                        adjust_day(&rtc);
                        break;
                    case screen::CHANGED:
                        // Handle CHANGED state if needed, or leave empty to suppress warning
                        break;
                    default:
                        break;
                }
                break;
            case screen::TIME_DISP:
                switch(lcd_screen.edit_time_stat)
                {
                    case screen::NO_EDIT:
                        lcd_screen.edit_time_stat = screen::EDIT_HOUR;
                        break;
                    case screen::EDIT_HOUR:
                        adjust_hour(&rtc);
                        break;
                    case screen::EDIT_MINUTE:
                        adjust_minute(&rtc);
                        break;
                    case screen::EDIT_SECOND:
                        adjust_second(&rtc);
                        break;
                    default:
                        break;
                }
                break;
            case screen::DRY_CALIBRATION:
            {
                lcd_screen.lcd->clear();
                lcd_screen.lcd->setCursor(0, 0);
                lcd_screen.lcd->print("Dry calibration");
                lcd_screen.lcd->setCursor(0, 1);
                lcd_screen.lcd->print("wait 30s");
                int new_cal_value = Pflanze->humidity_sensor_dry_calibration(humidity_sensor_pin);
                lcd_screen.lcd->clear();
                lcd_screen.calibrated_value_disp(true, new_cal_value);
                break;
            }
            case screen::WET_CALIBRATION:
            {
                lcd_screen.lcd->clear();
                lcd_screen.lcd->setCursor(0, 0);
                lcd_screen.lcd->print("Wet calibration");
                lcd_screen.lcd->setCursor(0, 1);
                lcd_screen.lcd->print("wait 30s");
                int new_value = Pflanze->humidity_sensor_wet_calibration(humidity_sensor_pin);
                lcd_screen.lcd->clear();
                lcd_screen.calibrated_value_disp(false, new_value);
                break;
            }
        }
	}
    if (lcd_screen.lit == false)
    {
        lcd_screen.lcd->noBacklight();
    }
    else
    {
        lcd_screen.lcd->backlight();
    }
	delay(100);
}

// für  RTC DS1302

// void next_button(TimeStruct water_time)
void next_button()
{
	lcd_screen.last_disp_action = millis();
	lcd_screen.lcd->backlight();
	TimeStruct now;
	// get the current time from the RTC
	get_time(&rtc, &now, rtc_available);

	if (lcd_screen.lit == true)
	{
        lcd_screen.prev_disp = screen::CHANGED;
		switch (lcd_screen.disp_status)
		{
			case screen::HOME:
				lcd_screen.disp_status = screen::WATER_DISP;
				break;
			case screen::WATER_DISP:
				lcd_screen.disp_status = screen::DRY_CALIBRATION;
				break;
			case screen::DRY_CALIBRATION:
            if (lcd_screen.calibrate == true){
                lcd_screen.calibrate = false;
            }
				lcd_screen.disp_status = screen::WET_CALIBRATION;
                break;
			case screen::WET_CALIBRATION:
				if (lcd_screen.calibrate == true){
                    lcd_screen.calibrate = false;
                }
                lcd_screen.disp_status = screen::DATE_DISP;
				break;
			case screen::DATE_DISP:
                
                switch(lcd_screen.edit_time_stat)
                {
                    case screen::EDIT_YEAR:
                        lcd_screen.edit_time_stat = screen::EDIT_MONTH;
                        break;
                    case screen::EDIT_MONTH:
                        lcd_screen.edit_time_stat = screen::EDIT_DAY;
                        break;
                    case screen::EDIT_DAY:
                        lcd_screen.disp_status = screen::TIME_DISP;
                        lcd_screen.edit_time_stat = screen::NO_EDIT;
                        break;
                    default:
                        lcd_screen.disp_status = screen::TIME_DISP;
                        lcd_screen.edit_time_stat = screen::NO_EDIT;
                        break;
                }
				break;
                //if year, month date are 
            
			case screen::TIME_DISP:
				
                switch(lcd_screen.edit_time_stat)
                {
                    case screen::EDIT_HOUR:
                        lcd_screen.edit_time_stat = screen::EDIT_MINUTE;
                        break;
                    case screen::EDIT_MINUTE:
                        lcd_screen.edit_time_stat = screen::EDIT_SECOND;
                        break;
                    case screen::EDIT_SECOND:
                        lcd_screen.edit_time_stat = screen::NO_EDIT;
                        lcd_screen.disp_status = screen::HOME;
                        break;
                    default:
                        lcd_screen.disp_status = screen::HOME;
                        lcd_screen.edit_time_stat = screen::NO_EDIT;
                        break;
                }
                break;
			default:
				lcd_screen.disp_status = screen::STARTUP;
				break;
		}
	}
	next_button_pressed = false;
	lcd_screen.lit = true;
	delay(100);
}