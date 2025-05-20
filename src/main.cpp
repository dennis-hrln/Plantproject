/*
Arduino pin wiring:
*  - Pin 2: Next button (input)
*  - Pin 3: Select button (input)
*  - Pin 4: RTC RST pin (input?)
*  - Pin 5: RTC CLK pin (input?)
*  - Pin 6: RTC DAT pin (output)
*  - Pin 7: Motor pin (output)



*  - Pin 10: SD card CS pin (output)
*  - Pin 11: SD card MOSI pin (input?)
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

#include "pflanzenliste.h"
#include "screen.h"
#include "plant.h"
#include "clock_rtc.h"
#include "buttons.h"
#include "sd_card.h"


//changable variables
///////////////////////////////
plant thisplant = Stirps; // what plant is used
int wet_sensor_value = 186;
int dry_sensor_value = 505;
float fps = 1; // frames per second 
int Water_amount = 20; // amount of water 
//hardware (pin numbers)
const byte nextButtonPIN = 2;
const byte selectButtonPIN = 3;
const byte water_pump_pin = 7;
const byte sd_card_pin = 10;
const byte humidity_sensor_pin = A0;
bool sd_card_used = true; // does this project use a sd card?


// int dry_sensor_value = 380;
////////////////////////////////



//Hardware Initialisation
LiquidCrystal_I2C lcd(0x27, 16, 2);
Ds1302 rtc(4, 5, 6);
SD_Card micro_sd(sd_card_pin, 5000);
// RTC_DS3231 rtc;
// byte rtc;		//debug or rtc is not existant


screen lcd_screen(&lcd);
bool humidity_control(plant *);
void calibration();
volatile bool next_button_pressed = false;
volatile bool select_button_pressed = false;
bool rtc_available;
unsigned long last_frame_time;

// use dry and wet values from this code for initialisation?
bool dry_calibrated = true;
bool wet_calibrated = true;

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




void setup()
{
	// initialize serial communications at 9600 bps:
	Serial.begin(9600);
	// Set pinmodes
	pinMode(humidity_sensor_pin, INPUT);
	pinMode(water_pump_pin, OUTPUT);
	pinMode(sd_card_pin, OUTPUT);
	pinMode(nextButtonPIN, INPUT_PULLUP);
	pinMode(selectButtonPIN, INPUT_PULLUP);

	// SD.begin(sd_card_pin);

	lcd_screen.innit();
	rtc_available = starting_up(&rtc);
	// micro_sd.initialize_SD_card();
	

	attachInterrupt(digitalPinToInterrupt(nextButtonPIN), check_next_button, RISING);
	attachInterrupt(digitalPinToInterrupt(selectButtonPIN), check_select_button, RISING);
	// to calibrate the sensor remove the two int in the function
	thisplant.calibrate_humidity_sensor(wet_sensor_value, dry_sensor_value);

}

void loop()
{
	
	bool was_watered = false;
	TimeStruct watering_time, time_now;	
	

	lcd_screen.screen_dimming(thisplant.planttype, thisplant.get_humidity(), thisplant.optimal_humidity);
	
	//thisplant.write_to_pc(data_frequency, &thisplant);

	// check if the plant needs watering and water it if needed; get watering time as dt
	was_watered = humidity_control(&thisplant);
	if (was_watered == true)
	{
		get_time(&rtc, &watering_time, rtc_available);
	}
	calibration();
	if (next_button_pressed)
	{
		next_button();
		lcd_screen.blinking = false; //turn off blinking
		last_frame_time = 0; //ensure that the screen is updated after the button is pressed
	}
	if (select_button_pressed)
	{
		lcd_screen.blinking = false; //turn off blinking
		select_button(); //ensure that the screen is updated after the button is pressed
		last_frame_time = 0;
	}
	// check if the RTC is available and get the current time
	get_time(&rtc, &time_now, rtc_available);
	// update the screen with the current time and date
	if (sd_card_used == true){ // && (millis() - micro_sd.last_data_write > micro_sd.data_frequency && millis()> 5000)
		micro_sd.write_to_SDcard(&thisplant, &time_now, sd_card_pin);
	}
	if (last_frame_time + (1000/fps) < millis())
	{
	lcd_screen.update_screen(
		thisplant.planttype, thisplant.get_humidity(), thisplant.optimal_humidity,
		watering_time.year, watering_time.month, watering_time.day,
		watering_time.hour, watering_time.minute, time_now.year, time_now.month, time_now.day, time_now.hour, time_now.minute, time_now.second
		);
		last_frame_time = millis();
	}
	
	
	
}

bool humidity_control(plant *Pflanze)
{
	//if plant was watered
	bool was_watered = false;
	// check if the plant needs watering
	float h_diff = Pflanze->measure_humidity(humidity_sensor_pin);
	if (h_diff > 0)
	{
		Pflanze->watering(h_diff, water_pump_pin, Water_amount);
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
		int new_cal_value = thisplant.humidity_sensor_dry_calibration(humidity_sensor_pin);
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
		int new_cal_value = thisplant.humidity_sensor_wet_calibration(humidity_sensor_pin);
		wet_calibrated = true;
		lcd_screen.lcd->clear();
		lcd_screen.calibrated_value_disp(false, new_cal_value);
	}
}

