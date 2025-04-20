/*
Arduino pin wiring:
*  - Pin 2: Next button (input)
*  - Pin 3: Select button (input)
*  - Pin 4: RTC RST pin (input?)
*  - Pin 5: RTC CLK pin (input?)
*  - Pin 6: RTC DAT pin (output)
*  - Pin 7: Motor pin (output)

*  - Pin 10: SD card MOSI pin (input?)
*  - Pin 11: SD card CS pin (input?)
*  - Pin 12: SD card MISO pin (input?)
*  - Pin 13: SD card CLK pin (input?)

*  - Pin A0: Humidity sensor pin (input)

*  - Pin A4: LCD SDA pin (output)
*  - Pin A5: LCD SLC pin (output)

5V: LCD, RTC
3V: SD card, Motor, Humidity sensor

*/
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <RTClib.h>
#include <Ds1302.h>

// #include "pflanzenliste.h"
#include "screen.h"
#include "plant.h"
#include "clock_rtc.h"
#include "buttons.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
// RTC_DS3231 rtc;

Ds1302 rtc(4, 5, 6); // Pin 4 is the rst pin, pin 6 is the DAT pin, pin  5 is the clock pin CLK pin


// byte rtc;		//debug or rtc is not existant

screen lcd_screen(&lcd);

bool humidity_control(plant *);
void calibration();

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
		// next_button(watering_time.year, watering_time.month, watering_time.day,
		// watering_time.hour, watering_time.minute);
	
		// next_button(2024, 12, 25, 9, 26);
		next_button();
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

