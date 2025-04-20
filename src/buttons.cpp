//programm multiole button functions with different Datetime formats for every rtc type
/* ex. DateTime now = rtc->now();   //create a DateTime object
Ds1302::DateTime dt = {
    .year = 25,
    .month = 1,
    .day = 1,
    .hour = 1,
    .minute = 2,
    .second = 3,
    // .dow = calculateDayOfWeek(.year, .month, .day);
    .dow = 1 // 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
};
*/

#include "buttons.h"
extern Ds1302 rtc; // or Ds1302 rtc, depending on your RTC type
// extern RTC_DS3231 rtc; // or Ds1302 rtc, depending on your RTC type
extern bool rtc_available;
extern bool dry_calibrated;
extern bool wet_calibrated;
extern volatile bool next_button_pressed;
extern volatile bool select_button_pressed;
extern screen lcd_screen;

void next_button(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute)
{
	lcd_screen.last_disp_action = millis();
	lcd_screen.lcd->backlight();
	TimeStruct now;
	// get the current time from the RTC
	get_time(&rtc, &now, rtc_available);

	if (lcd_screen.lit == true)
	{
		switch (lcd_screen.disp_status)
		{
			case screen::HOME:
				lcd_screen.water_disp(year, month, day, hour, minute );
				lcd_screen.disp_status = screen::WATER_DISP;
				break;
			case screen::WATER_DISP:
				lcd_screen.disp_status = screen::DRY_CALIBRATION;
				lcd_screen.calibration_disp();
				break;
			case screen::DRY_CALIBRATION:
				lcd_screen.disp_status = screen::WET_CALIBRATION;
				lcd_screen.calibration_disp();
				break;
			case screen::WET_CALIBRATION:
				lcd_screen.disp_status = screen::DATE_DISP;
				lcd_screen.date_disp(now.year, now.month, now.day, now.hour, now.minute, now.second);
				break;
			case screen::DATE_DISP:
				lcd_screen.disp_status = screen::TIME_DISP;
				lcd_screen.date_disp(now.year, now.month, now.day, now.hour, now.minute, now.second);
				break;
			case screen::TIME_DISP:
				lcd_screen.disp_status = screen::HOME;
				lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
				break;
			default:
				lcd_screen.lcd->clear();
				lcd_screen.lcd->setCursor(0, 0);
				lcd_screen.lcd->print(lcd_screen.disp_status);
				lcd_screen.lcd->print("startup");
				lcd_screen.disp_status = screen::HOME;
				delay(2000);
				lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
				break;
		}
	}
	next_button_pressed = false;
	lcd_screen.lit = true;
	delay(100);
}

void select_button()
{
	lcd_screen.last_disp_action = millis();
	lcd_screen.lcd->backlight();
	if (lcd_screen.lit == true)
	{
		if (lcd_screen.disp_status == screen::DRY_CALIBRATION)
		{
			dry_calibrated = false;
		}
		else if (lcd_screen.disp_status == screen::WET_CALIBRATION)
		{
			wet_calibrated = false;
		}
		if (lcd_screen.disp_status == screen::WATER_DISP)
		{
			reset_time(&rtc);
		}
		else if (lcd_screen.disp_status == screen::HOME)
		{
			lcd_screen.lit = false;
			lcd_screen.lcd->noBacklight();
			lcd_screen.disp_status = screen::HOME;
		}
		else
		{
			lcd_screen.lit = false;
			lcd_screen.lcd->noBacklight();
			lcd_screen.disp_status = screen::HOME;
		}
	}
	lcd_screen.lit = true;
	select_button_pressed = false;
	delay(100);
}

// für  RTC DS1302

void next_button(TimeStruct water_time)
{
	lcd_screen.last_disp_action = millis();
	lcd_screen.lcd->backlight();
	TimeStruct now;
	// get the current time from the RTC
	get_time(&rtc, &now, rtc_available);

	if (lcd_screen.lit == true)
	{
		switch (lcd_screen.disp_status)
		{
			case screen::HOME:
				lcd_screen.water_disp(water_time.year, water_time.month, water_time.day, water_time.hour, water_time.minute );
				lcd_screen.disp_status = screen::WATER_DISP;
				break;
			case screen::WATER_DISP:
				lcd_screen.disp_status = screen::DRY_CALIBRATION;
				lcd_screen.calibration_disp();
				break;
			case screen::DRY_CALIBRATION:
				lcd_screen.disp_status = screen::WET_CALIBRATION;
				lcd_screen.calibration_disp();
				break;
			case screen::WET_CALIBRATION:
				lcd_screen.disp_status = screen::DATE_DISP;
				lcd_screen.date_disp(now.year, now.month, now.day, now.hour, now.minute, now.second);
				break;
			case screen::DATE_DISP:
				lcd_screen.disp_status = screen::TIME_DISP;
				lcd_screen.date_disp(now.year, now.month, now.day, now.hour, now.minute, now.second);
				break;
			case screen::TIME_DISP:
				lcd_screen.disp_status = screen::HOME;
				lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
				break;
			default:
				lcd_screen.lcd->clear();
				lcd_screen.lcd->setCursor(0, 0);
				lcd_screen.lcd->print(lcd_screen.disp_status);
				lcd_screen.lcd->print("startup");
				lcd_screen.disp_status = screen::HOME;
				delay(2000);
				lcd_screen.home_disp(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
				break;
		}
	}
	next_button_pressed = false;
	lcd_screen.lit = true;
	delay(100);
}