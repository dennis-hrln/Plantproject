#include <Arduino.h>
#include "plant.h"
#include <SD.h>


plant::plant(String planttype, float optimal_humidity, const int arduino_sensor_pin, const int arduino_motor_pin, const int arduino_SD_card_pin)
{
    this->planttype = planttype;
    this->optimal_humidity = optimal_humidity;
    this->sensor_pin = arduino_sensor_pin;
    this->motor_pin = arduino_motor_pin;
    this->last_data_write = 0;  //initialize the last data write time to 0
    this->SD_card_pin = arduino_SD_card_pin;

    // this->measure_humidity = Funktion;
};

float plant::measure_humidity()
{
    int humidity_sensor_Value = analogRead(this->sensor_pin);
    this->humidity = map(humidity_sensor_Value, 0, 1023, 0, 100);
    humidity_difference = this->optimal_humidity - this->humidity;

    return humidity_difference;
};

void plant::watering(float humidity_difference, int water_amount = 30) // water_amount is an example value
{
    if (humidity_difference > 0)
    {
        if (millis() - this->last_watered > 20000)
        {
            digitalWrite(motor_pin, HIGH);
            delay(100 * water_amount);
            digitalWrite(motor_pin, LOW);
            this->last_watered = millis();
            this->last_data_write = 0;
            this->watered = true;
            write_to_SDcard(this->SD_card_pin);
            this->watered = false;


        }
        
    }
};


//initialisation of the SD card I'll write the data to
void plant::write_to_SDcard(unsigned long measurment_frequency)
{
    /*static bool init is used to make sure that the file is only created once
    because it is static it will keep its value between function calls
    and will not be reinitialised every time the function is called*/
    static bool init = false;

    if (!SD.begin(this->SD_card_pin)) {
        Serial.println(F("SD Card initialization failed!"));
        return;
    }

    file = SD.open((this->planttype + "log.csv"), FILE_WRITE);
    //if the file is not initialised yet -> titles for csv file (if init == false)
    if (!init)
    {
        //add date to the file name so every time the program is started a new file is created
        //#ToDo
        
        if (file)
        {
            //print the names of the data to the file in csv format
            for (int i = 0; i < sizeof(data_names) / sizeof(data_names[0]); i++)
            {
                file.print(this->data_names[i]);
                file.print(F(", "));
            }
            file.println(); // Add newline after headers
            init = true;
        }
        else
        {
            Serial.print(F("SD Card: error on opening file.\n"));
        }
    }

    else
    {
        if (millis() - this->last_data_write  < measurment_frequency)
        {
            return;
        }

        if (file)
        {
             //print the data to the file in csv format

            file.println ((String(millis()) + ", " +  this->planttype + ", " + String(this->optimal_humidity) 
            + ", " + String(this->humidity) + ", " + String(this->watered) + "\n"));
            this->last_data_write = millis();
        }
        else
        {
            Serial.print(F("SD Card: error on opening file. \n"));
        }
    }
    file.close();
}
