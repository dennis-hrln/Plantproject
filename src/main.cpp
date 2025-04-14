#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <RTClib.h>

#include "pflanzenliste.h"
#include "screen.h"
#include "plant.h"
#include "clock_rtc.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
screen lcd_screen(&lcd);

bool humidity_control(plant *);
void calibration();
void next_button(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void select_button();
unsigned long data_frequency = 5000; // time in ms between data writes to the SD card
volatile bool next_button_pressed = false;
volatile bool select_button_pressed = false;
const byte nextButtonPIN = 2;
const byte selectButtonPIN = 3;
bool dry_calibrated = true;
bool wet_calibrated = true;
bool rtc_available;


void check_next_button()
{
	next_button_pressed = true;
	// Serial.println(F("next button pressed"));
}
void check_select_button()
{
	// Serial.println(F("select button pressed"));
	select_button_pressed = true;
}

int wet_sensor_value = 186;
int dry_sensor_value = 380;

void setup()
{
	// initialize serial communications at 9600 bps:
	Serial.begin(9600);
	SD.begin(Stirps.SD_card_pin);

	lcd_screen.innit();
	rtc_available = starting_up(&rtc);


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
	rtc_available = starting_up(&rtc);///////////////////////////////////////////////////////////

	// local variables in loop
	bool was_watered = false;
	TimeStruct watering_time, time_now;	

	lcd_screen.screen_dimming(Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity);
	Stirps.write_to_SDcard(data_frequency);
	//Stirps.write_to_pc(data_frequency);

	// check if the plant needs watering and water it if needed; get watering time as dt
	was_watered = humidity_control(&Stirps);
	if (was_watered == true)
	{
		get_time(&rtc, &watering_time, rtc_available);
	}
	calibration();
	if (next_button_pressed)
	{
		next_button(watering_time.year, watering_time.month, watering_time.day,
		watering_time.hour, watering_time.minute);
	}
	if (select_button_pressed)
	{
		select_button();
	}
	// check if the RTC is available and get the current time
	get_time(&rtc, &time_now, rtc_available);
	// update the screen with the current time and date
	lcd_screen.update_screen(
		Stirps.planttype, Stirps.humidity, Stirps.optimal_humidity,
		watering_time.year, watering_time.month, watering_time.day,
		watering_time.hour, watering_time.minute, time_now.year, time_now.month, time_now.day, time_now.hour, time_now.minute, time_now.second
		);
}

bool humidity_control(plant *Pflanze)
{
	//if plant was watered
	bool was_watered = false;
	// check if the plant needs watering
	float h_diff = Pflanze->measure_humidity();
	if (h_diff > 0)
	{
		Pflanze->watering(h_diff, 10);
		was_watered = true;
	}
	return was_watered;

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
		int new_cal_value = Stirps.humidity_sensor_dry_calibration();
		dry_calibrated = true;
		lcd_screen.lcd->clear();
		lcd_screen.calibrated_value_disp(true, new_cal_value);
	}
	if (wet_calibrated == false)
	{
		lcd_screen.lcd->clear();
		lcd_screen.lcd->setCursor(0, 0);
		lcd_screen.lcd->print("Wet calibration");
		lcd_screen.lcd->setCursor(0, 1);
		lcd_screen.lcd->print("wait 30s");
		int new_cal_value = Stirps.humidity_sensor_wet_calibration();
		wet_calibrated = true;
		lcd_screen.lcd->clear();
		lcd_screen.calibrated_value_disp(false, new_cal_value);
	}
}

void next_button(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute)
{
	lcd_screen.last_disp_action = millis();
	lcd_screen.lcd->backlight();
	TimeStruct now;
	// get the current time from the RTC
	get_time(&rtc, &now, rtc_available);

	if (lcd_screen.lit == true)
	{
		if (lcd_screen.disp_status == "home")
		{
			lcd_screen.water_disp(year, month, day, hour, minute );
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
		
			lcd_screen.disp_status = "date_disp";
			lcd_screen.date_disp(now.year, now.month, now.day, now.hour, now.minute, now.second);
			
		}
		else if (lcd_screen.disp_status == "date_disp")
		{
			lcd_screen.disp_status = "time_disp";
			lcd_screen.date_disp(now.year, now.month, now.day, now.hour, now.minute, now.second);
		}
		else if (lcd_screen.disp_status == "time_disp")
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
	lcd_screen.lit = true;
	delay(100);
}

void select_button()
{
	lcd_screen.last_disp_action = millis();
	lcd_screen.lcd->backlight();
	if (lcd_screen.lit == true)
	{
		if (lcd_screen.disp_status == "dry_calibration")
		{
			dry_calibrated = false;
		}
		else if (lcd_screen.disp_status == "wet_calibration")
		{
			wet_calibrated = false;
		}
	}
	lcd_screen.lit = true;
	select_button_pressed = false;
	delay(100);
}