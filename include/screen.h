#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class screen
{

public:
	screen(LiquidCrystal_I2C *LCD);
	LiquidCrystal_I2C *lcd;

	void home_disp(const char *plantname, int humidity, int optimal_humidity);										  // Display humidity /optimal humdiity
	void water_disp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute); // Display the last time the plant was watered
	void calibration_disp();
	void date_disp(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
	unsigned long stay_awake_time = long(60) * long(1000);							// in ms
	void screen_dimming(const char *plantname, int humidity, int optimal_humidity); // Turns off the backlight after a certain time
	void calibrated_value_disp(bool is_dry_calibration, int new_value);				// Display the new value of the humidity sensor
	void innit();
	void update_screen(const char *plantname, int humidity, int optimal_humidity,
					   unsigned int H2Oyear, unsigned int H2Omonth, unsigned int H2Oday, unsigned int H2Ohour, unsigned int H2Ominute,
					   unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second);
	unsigned long last_disp_action = 0;
	enum DispStatus
	{
		STARTUP,
		HOME,
		WATER_DISP,
		DRY_CALIBRATION,
		WET_CALIBRATION,
		DATE_DISP,
		TIME_DISP,
		CHANGED
	}; // Display status enum
	// create a instace of the enum -> used in the code
	DispStatus disp_status;	// current display status
	DispStatus prev_disp;	// previous display status
	enum Edit_Time_status
	{
		NO_EDIT,
		EDIT_YEAR,
		EDIT_MONTH,
		EDIT_DAY,
		EDIT_HOUR,
		EDIT_MINUTE,
		EDIT_SECOND
	}; // Display status enum
	Edit_Time_status edit_time_stat;
	unsigned long last_disp_change = 0;
	bool lit = true;
	bool blinking; // checks if the cursor is blinking

private:
bool avoid_negative_humidity = true; //turn off for testing
bool check_for_change(); // check if the display has changed - does not check for changing content (like date and time) only for disp changes (home->time, etc.)
	int prev_humidity; // previous humidity value
	char prev_plantname[20]; // previous plant name
	unsigned int refresh_rate = 300; // Display frequency enum
	unsigned long hum_refresh_rate = 1000; // Refresh rate for humidity display
	unsigned long hum_last_refresh = 0; // Last refresh time for humidity display
	
	// Custom characters

	// byte right_arrow[8] = {
	//     0b00000,
	//     0b00000,
	//     0b00000,
	//     0b00010,
	//     0b11111,
	//     0b00010,
	//     0b00000,
	//     0b00000
	// };
	// byte down_arrow[8] = {
	//     0b00100,
	//     0b00100,
	//     0b00100,
	//     0b00100,
	//     0b00100,
	//     0b11111,
	//     0b01110,
	//     0b00100
	// };
	// byte up_arrow[8] = {
	//     0b00100,
	//     0b01110,
	//     0b11111,
	//     0b00100,
	//     0b00100,
	//     0b00100,
	//     0b00100,
	//     0b00100
	// };
	// byte left_arrow[8] = {
	//     0b00000,
	//     0b00000,
	//     0b00000,
	//     0b01000,
	//     0b11111,
	//     0b01000,
	//     0b00000,
	//     0b00000
	// };
	byte select_char[8] = {
		B00000,
		B00100,
		B01110,
		B11111,
		B11111,
		B01110,
		B00100,
		B00000};
};

#endif // SCREEN_H