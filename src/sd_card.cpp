// initialisation of the SD card I'll write the data to
#include "sd_card.h"
SD_Card::SD_Card(int pin, unsigned long data_write_frequency)
{
    this->SD_card_pin = pin;
    this->data_frequency = data_write_frequency; // set the data write frequency to 5 seconds
    this->last_data_write = 0;                   // initialize the last data write time to 0
}

void SD_Card::write_to_SDcard(plant *plant_instance, TimeStruct *now)
{
    // the plant instance is used to get the data from the plant class (for example the humidity from Stirps)
    if (millis() - this->last_data_write > this->data_frequency && millis()> 5000)
    {

        int Year = now->year;
        int Month = now->month;
        int Day = now->day;
        int Hour = now->hour;
        int Minute = now->minute;
        int Second = now->second;
        if (Year < 999)
        {
            Year += 2000;
        }

        if (!SD.begin(this->SD_card_pin))
        {
            // error message
            Serial.println(F("SD Card initialization failed!"));
            return;
        }
        Serial.println(F("SD Card initialized."));
        // open file for writing
        
        snprintf(filename, sizeof(filename), "%s_%04d.%02d.%02d_data.csv", plant_instance->planttype,
                 Year, Month, Day);
        Serial.print(filename);
        file = SD.open(filename, FILE_WRITE);
        Serial.println(F("SD Card opened."));
        // if the file is not initialised yet -> titles for csv file (if init == false)
        if (!this->init)
        {

            if (file)
            {
                // print the names of the data to the file in csv format
                for (int i = 0; i < static_cast<int>(sizeof(data_names) / sizeof(data_names[0])); i++)
                {
                    file.print(this->data_names[i]);
                    file.print(F(", "));
                }
                file.println(); // Add newline after headers
                init = true;
                file.close();
            }
            else
            {
                // error message
                Serial.println(F("Error wrtiting to SD card!"));
            }
        }

        else
        {

            if (file)
            {
                // Convert bool to string representation
                const char *watered_str = plant_instance->watered ? "true" : "false";

                // print the data to the file in csv format
                char data[128]; // buffer for the data to be written to the file
                // create one giant string with all the data in it
                snprintf(data, sizeof(data), "%04d,%02d,%02d,%02d,%02d,%02d,%s,%02d,%02d,%s",
                         Year, Month, Day, Hour, Minute, Second,
                         plant_instance->planttype, plant_instance->optimal_humidity, plant_instance->humidity, watered_str);

                file.println(data);
                this->last_data_write = millis();
                plant_instance->watered = false; // reset the watered variable to false after writing to the file
                file.close(); // close the file after writing to it
            }
            else
            {
                Serial.println(F("Error wrtiting to SD card!"));
            }
        }
        
    }
    else
    {
        Serial.println(F("SD Card not initialized!"));
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
        Serial.print(plant_instance->humidity);
        Serial.print(F(", "));
        Serial.print(plant_instance->watered);
        this->last_data_write = millis();
    }
}
