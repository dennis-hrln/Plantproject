#ifndef SD_CARD_H
#define SD_CARD_H
#include <Arduino.h>
#include <SD.h>
#include "plant.h"
#include "clock_rtc.h"
class SD_Card
{
public:
    SD_Card(int arduino_SD_card_pin, unsigned long data_write_frequency = 60000);
    void write_to_SDcard(plant* plant_instance = nullptr, TimeStruct* now = nullptr);
    void write_to_pc(unsigned long measurment_frequency = 2e4 , plant* plant_instance = nullptr);
    unsigned long data_frequency; // frequency of the data write in ms
    unsigned long last_data_write;

private:
    bool init = false; // check if the SD card is initialized (also possible with static var inside the function)
    File file;
    char filename[32]; // name of the file
    
    int SD_card_pin;
    
    const char data_names[100] = {"year, month, day, hour, minute, second, planttype, optimal_humidity [%], humidity[%], watered"};
};

#endif // SD_CARD_H