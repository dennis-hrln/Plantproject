// initialisation of the SD card I'll write the data to
#include "sd_card.h"

SD_Card::SD_Card(int pin, unsigned long data_write_frequency)
{
    this->SD_card_pin = pin;
    this->data_frequency = data_write_frequency; // set the data write frequency to 5 seconds
    this->last_data_write = 0;                   // initialize the last data write time to 0
}

void SD_Card::initialize_SD_card()
{
    static bool initialized;
    if (!initialized) {
        if(SD.begin(this->SD_card_pin)){
            initialized = true;
            Serial.println(F("SD Card initialized!"));
        }
        else
        {
            Serial.println(F("SD Card not initialized!"));
            return;
        }
    }
}
void SD_Card::write_to_SDcard(plant *plant_instance, TimeStruct *now)
{
    
    
    // the plant instance is used to get the data from the plant class (for example the humidity from Stirps)
    if ((millis() - this->last_data_write > this->data_frequency) && millis()> 5000)
    {
        initialize_SD_card();
        int Year = now->year;
        int Month = now->month;
        int Day = now->day;
        int Hour = now->hour;
        int Minute = now->minute;
        int Second = now->second;
        if (Year < 1000)
        {
            Year += 2000;
        }

        // open file for writing 24.09.24
        //filename in sd module max 12 chars
        // snprintf(filename, sizeof(filename), "%s_%04d.%02d.%02d_data.csv", plant_instance->planttype,
        //          Year, Month, Day);
        // Serial.print(filename);
        snprintf(filename, 13, "%02d_%02d_%02d.csv", (Year -2000), Month, Day);
        File file = SD.open(filename, FILE_WRITE);

        // if the file is not initialised yet -> titles for csv file (if init == false)
        if (!this->init)
        {
            file.print(data_names);
            file.println(); // Add newline after headers
            init = true;
            file.close();
        }

        else
        {

            // if (file)
            // {
                // Convert bool to string representation
            const char *watered_str = plant_instance->watered ? "true" : "false";

                // print the data to the file in csv format
                // create one giant string with all the data in it
            file.print(Year);
            file.print(F(", "));
            file.print(Month);
            file.print(F(", "));
            file.print(Day);
            file.print(F(", "));
            file.print(Hour);
            file.print(F(", "));
            file.print(Minute);
            file.print(F(", "));
            file.print(Second);
            file.print(F(", "));
            file.print(plant_instance->planttype);
            file.print(F(", "));
            file.print(plant_instance->optimal_humidity);
            file.print(F(", "));
            file.print(plant_instance->get_humidity());
            file.print(F(", "));
            file.print(watered_str);
            file.println(); // Add newline after data
            this->last_data_write = millis();
            plant_instance->watered = false; // reset the watered variable to false after writing to the file
            file.close(); // close the file after writing to it
            // }
        }    
    }
    
}

void SD_Card::write_to_pc(unsigned long data_frequency, plant *plant_instance)
{
    static bool initiation = true;

    if (initiation)
    {

        Serial.println(F("runtime[s], planttype, optimal_humidity, humidity, watered"));
        initiation = false;
    }

    else
    {
        if (millis() - this->last_data_write < data_frequency)
        {
            return;
        }
        {
            return;
        }
        {
            return;
        }
        Serial.println();
        Serial.print(millis() / 1000);
        Serial.print(F(", "));
        Serial.print(plant_instance->planttype);
        Serial.print(F(", "));
        Serial.print(plant_instance->optimal_humidity);
        Serial.print(F(", "));
        Serial.print(plant_instance->get_humidity());
        Serial.print(F(", "));
        Serial.print(plant_instance->watered);
        this->last_data_write = millis();
    }
}
