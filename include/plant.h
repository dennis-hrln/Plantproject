#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>
#include <SD.h>

class plant
{
public:
    plant(const char* planttype, int optimal_humidity = 25);

    const char* planttype;
    int optimal_humidity;
    
    
    bool watered = false;
    //calibrate the humidity sensor to get the coorect dry value
    int humidity_sensor_dry_calibration(int sensor_pin = A0);
    //calibrate the humidity sensor to get the coorect wet value
    int humidity_sensor_wet_calibration(int sensor_pin = A0);
    void calibrate_humidity_sensor(int, int);
    void watering(int motor_pin = 7, int water_amount = 50);
    int measure_humidity(int sensor_pin = A0);
    //getter and setter functions (so vars can be private)
    int get_humidity();
    bool humidity_control(int humidity_sensor_pin = A0, int water_pump_pin = 7, int water_amount = 50, int time_between_watering = 35000);

    

    
    
    unsigned long last_watered = 0;
private:
    int sensor_pin;
    int motor_pin;
    int SD_card_pin;
    int humidity;
    int sensor_wet;
    int sensor_dry;
};

#endif // PLANT_H