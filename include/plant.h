#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>
#include <SD.h>

class plant
{
public:
    plant(const char* planttype, int optimal_humidity = 25, const int sensor_pin = 0, const int motor_pin = LED_BUILTIN, const int SD_card_pin = 0);

    const char* planttype;
    int optimal_humidity;
    
    
    bool watered = false;
    
    int humidity_sensor_dry_calibration();
    int humidity_sensor_wet_calibration();
    void calibrate_humidity_sensor(int, int);
    void watering(float, int  water_amount = 50);
    int measure_humidity();
    //getter and setter functions (so vars can be private)
    int get_humidity();
    int get_sensor_pin();
    int get_motor_pin();
    int get_SD_card_pin();
    

    
    
    unsigned long last_watered = 0;
private:
    int sensor_pin;
    int motor_pin;
    int SD_card_pin;
    int humidity;
    int sensor_wet;
    int sensor_dry;
    int humidity_difference;
};

#endif // PLANT_H