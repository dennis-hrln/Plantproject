#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>
#include <SD.h>

class plant
{
public:
    plant(String planttype, int optimal_humidity = 25, const int sensor_pin = 0, const int motor_pin = LED_BUILTIN, const int SD_card_pin = 0);

    String planttype;
    int optimal_humidity;
    int humidity;
    int humidity_difference;

    void calibrate_humidity_sensor();
    void calibrate_humidity_sensor(int, int);
    void watering(float, int);
    int measure_humidity();
    void write_to_SDcard(unsigned long = 60000);
    void write_to_pc(unsigned long measurmentu_freqency = 2e4);
    int sensor_pin;
    int motor_pin;
    int SD_card_pin;

private:
    File file;
    unsigned long last_data_write;
    const String data_names[5] = {"runtime[s]", "planttype", "optimal_humidity [%]", "humidity[%]", "watered"};
    unsigned long last_watered;
    bool watered;
    int sensor_wet;
    int sensor_dry;
};

#endif // PLANT_H