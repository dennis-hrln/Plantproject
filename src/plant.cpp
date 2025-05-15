#include "plant.h"

plant::plant(const char* planttype, int optimal_humidity, const int arduino_sensor_pin, const int arduino_motor_pin, const int arduino_SD_card_pin)
{
    this->planttype = planttype;
    this->optimal_humidity = optimal_humidity;
    this->sensor_pin = arduino_sensor_pin;
    this->motor_pin = arduino_motor_pin;
    
    this->SD_card_pin = arduino_SD_card_pin;
    this->last_watered = 0;
    this->sensor_wet = 1023;
    this->sensor_dry = 0;

    // this->measure_humidity = Funktion;
};

// set the values for the wet and dry sensor values
void plant::calibrate_humidity_sensor(int wet, int dry)
{
    this->sensor_wet = wet;
    this->sensor_dry = dry;
}

int plant::humidity_sensor_dry_calibration()
{
    this->sensor_dry = 0; // reset the sensor_dry value to 0 before calibration
    int calibrating_time = 30 * 1000; // time for the calibration in ms
    // Serial.println("Take the sensor out of the soil, dry it and wait for " + String(calibrating_time / 1000) + " seconds.");
    // gets the lowest value
    unsigned long startime = millis();
    while (startime + calibrating_time > millis())
    {
        int hum_val = analogRead(this->sensor_pin);
        if (this->sensor_dry < hum_val)
        {
            this->sensor_dry = hum_val;
            delay(10);
            Serial.println(hum_val);
        }
        
    }
    return this->sensor_dry; // return the value of the dry sensor
};
int plant::humidity_sensor_wet_calibration()
{
    this->sensor_wet = 1023; // reset the sensor_wet value to 1023 before calibration
    int calibrating_time = 30 * 1000; // time for the calibration in ms
    unsigned long startime = millis();
    // Serial.println("Now put the sensor in water and wait for " + String(calibrating_time / 1000) + " seconds.");
    while (startime + calibrating_time > millis())
    {
        int hum_val = analogRead(this->sensor_pin);
        if (this->sensor_wet > hum_val)
        {
            this->sensor_wet = hum_val;
            delay(10);
            Serial.println(hum_val);
        }
    }
    return this->sensor_wet; // return the value of the wet sensor
    // calibrate the sensor
};

int plant::measure_humidity()
{
    int humidity_sensor_Value = analogRead(this->sensor_pin);
    this->humidity = map(humidity_sensor_Value, this->sensor_wet, this->sensor_dry, 100, 0);
    humidity_difference = this->optimal_humidity - this->humidity;

    return humidity_difference;
};

void plant::watering(float humidity_difference, int water_amount) // water_amount is an example value
{
    if (humidity_difference > 0)
    {
        if (millis() - this->last_watered > 20000)
        {
            digitalWrite(this->motor_pin, HIGH);
            delay(100 * water_amount);
            digitalWrite(this->motor_pin, LOW);
            this->last_watered = millis();
            this->watered = true;
            //write_to_pc();
            this->watered = false;
        }
    }
};

// getter and setter functions
int plant::get_humidity()
{
    return this->humidity;
};
int plant::get_sensor_pin()
{
    return this->sensor_pin;
};
int plant::get_motor_pin()
{
    return this->motor_pin;
};
int plant::get_SD_card_pin()
{
    return this->SD_card_pin;
};

