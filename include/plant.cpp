#include <Arduino.h>
#include "plant.h"
#include <SD.h>


plant::plant(String planttype, float optimal_humidity, const int arduino_sensor_pin, const int arduino_motor_pin)
{
    this->planttype = planttype;
    this->optimal_humidity = optimal_humidity;
    this->sensor_pin = arduino_sensor_pin;
    this->motor_pin = arduino_motor_pin;

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
    while (humidity_difference > 0)
    {
        analogWrite(motor_pin, HIGH);
        delay(100 * water_amount);
        analogWrite(motor_pin, LOW);
        delay(20000);
        humidity_difference = measure_humidity();
    }
};


//initialisation of the SD card I'll write the data to
void plant::write_to_SDcard(const int PIN_SPI_CS, bool init)
{
    if (init == true)
    {
        file = SD.open((this->planttype + "log.csv"), FILE_WRITE);
        if (file)
        {
            file.println(" Time, Planttype, Optimal Humidity, Humidity \n");    //Date data
            file.close();
        }
        else
        {
            Serial.print(F("SD Card: error on opening file.\n"));
        }
    }
}

//writing the data to the SD card as CSV
void plant::write_to_SDcard(const int PIN_SPI_CS, unsigned long measurment_frequency)
{
    file = SD.open((this->planttype + "log.csv"), FILE_WRITE);
    if (this->last_data_write - millis() < measurment_frequency)
    {
        return;
    }

    if (file) 
    {
        file.println ((millis() + ", " +  this->planttype + ", " + this->optimal_humidity + ", " + this->humidity + "\n"));
        file.close();
    }
    else
    {
        Serial.print(F("SD Card: error on opening file. \n"));
    }
}
