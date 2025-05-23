#include "plant.h"

plant::plant(const char* planttype, int optimal_humidity)
{
    this->planttype = planttype;
    this->optimal_humidity = optimal_humidity;
    
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

int plant::humidity_sensor_dry_calibration(int sensor_pin)
{
    this->sensor_dry = 0; // reset the sensor_dry value to 0 before calibration
    int calibrating_time = 30 * 1000; // time for the calibration in ms
    // Serial.println("Take the sensor out of the soil, dry it and wait for " + String(calibrating_time / 1000) + " seconds.");
    // gets the lowest value
    unsigned long startime = millis();
    while (startime + calibrating_time > millis())
    {
        int hum_val = analogRead(sensor_pin);
        if (this->sensor_dry < hum_val)
        {
            this->sensor_dry = hum_val;
            delay(10);
            Serial.println(hum_val);
        }
        
    }
    return this->sensor_dry; // return the value of the dry sensor
};
int plant::humidity_sensor_wet_calibration(int sensor_pin)
{
    this->sensor_wet = 1023; // reset the sensor_wet value to 1023 before calibration
    int calibrating_time = 30 * 1000; // time for the calibration in ms
    unsigned long startime = millis();
    // Serial.println("Now put the sensor in water and wait for " + String(calibrating_time / 1000) + " seconds.");
    while (startime + calibrating_time > millis())
    {
        int hum_val = analogRead(sensor_pin);
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

int plant::measure_humidity(int sensor_pin)
{
    int humidity_sensor_Value = analogRead(sensor_pin);
    this->humidity = map(humidity_sensor_Value, this->sensor_wet, this->sensor_dry, 100, 0);
    int humidity_difference = this->optimal_humidity - this->humidity;

    return humidity_difference;
};

void plant::watering(int motor_pin, int water_amount) // water_amount is an example value
{
        if (millis() - this->last_watered > 20000)
        {
            digitalWrite(motor_pin, HIGH);
            delay(100 * water_amount);
            digitalWrite(motor_pin, LOW);
            this->last_watered = millis();
            Serial.print("watered");
        }
};

// getter and setter functions
int plant::get_humidity()
{
    return this->humidity;
};

/*
check if the plant needs watering 
*/
bool plant::humidity_control(int sensor_pin, int motor_pin, int water_amount, int time_between_watering){
    int h_diff = measure_humidity(sensor_pin);
    bool was_watered;
    was_watered = false;
    if ((h_diff > 0) && (millis() - this->last_watered > time_between_watering)){
        watering(motor_pin, water_amount);
        was_watered = true;
    }
    return was_watered;
}

