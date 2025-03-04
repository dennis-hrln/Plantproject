#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>
#include <SD.h>

class plant
{
    public:
    plant(String planttype, float optimal_humidity = 25, const int sensor_pin = NULL, const int motor_pin = LED_BUILTIN, const int SD_card_pin = NULL);

    
    String planttype;
    float optimal_humidity;
    float humidity;
    float humidity_difference;
    

    void watering(float,int);
    float measure_humidity();
    void write_to_SDcard(unsigned long = 60000);

    private:
    int sensor_pin;
    int motor_pin;
    int SD_card_pin;
    File file;
    unsigned long last_data_write;
    const String data_names[5] = {"time", "planttype", "optimal_humidity", "humidity", "watered"};;
    unsigned long last_watered;
    bool watered;
};

#endif // PLANT_H