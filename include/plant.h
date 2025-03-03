#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>

class plant
{
    public:
    plant(String planttype, float optimal_humidity = 25, const int sensor_pin = NULL, const int motor_pin = LED_BUILTIN);

    
    String planttype;
    float optimal_humidity;
    float humidity;
    float humidity_difference;
    int sensor_pin;
    int motor_pin;

    void watering(float,int);
    float measure_humidity();


};

#endif // PLANT_H