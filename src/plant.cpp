#include "plant.h"

plant::plant(const char* planttype, int optimal_humidity, const int arduino_sensor_pin, const int arduino_motor_pin, const int arduino_SD_card_pin)
{
    this->planttype = planttype;
    this->optimal_humidity = optimal_humidity;
    this->sensor_pin = arduino_sensor_pin;
    this->motor_pin = arduino_motor_pin;
    this->last_data_write = 0; // initialize the last data write time to 0
    this->SD_card_pin = arduino_SD_card_pin;
    this->last_watered = 0;
    this->sensor_wet = 1023;
    this->sensor_dry = 0;

    // this->measure_humidity = Funktion;
};

// set the values for the wet and dry sensor values
void plant::calibrate_humidity_sensor(int wet, int dry)
{
    this->sensor_wet = wet;
    this->sensor_dry = dry;
}

int plant::humidity_sensor_dry_calibration()
{
    this->sensor_dry = 0; // reset the sensor_dry value to 0 before calibration
    int calibrating_time = 30 * 1000; // time for the calibration in ms
    // Serial.println("Take the sensor out of the soil, dry it and wait for " + String(calibrating_time / 1000) + " seconds.");
    // gets the lowest value
    unsigned long startime = millis();
    while (startime + calibrating_time > millis())
    {
        int hum_val = analogRead(this->sensor_pin);
        if (this->sensor_dry < hum_val)
        {
            this->sensor_dry = hum_val;
            delay(10);
            Serial.println(hum_val);
        }
        
    }
    return this->sensor_dry; // return the value of the dry sensor
};
int plant::humidity_sensor_wet_calibration()
{
    this->sensor_wet = 1023; // reset the sensor_wet value to 1023 before calibration
    int calibrating_time = 30 * 1000; // time for the calibration in ms
    unsigned long startime = millis();
    // Serial.println("Now put the sensor in water and wait for " + String(calibrating_time / 1000) + " seconds.");
    while (startime + calibrating_time > millis())
    {
        int hum_val = analogRead(this->sensor_pin);
        if (this->sensor_wet > hum_val)
        {
            this->sensor_wet = hum_val;
            delay(10);
            Serial.println(hum_val);
        }
    }
    return this->sensor_wet; // return the value of the wet sensor
    // calibrate the sensor
};

int plant::measure_humidity()
{
    int humidity_sensor_Value = analogRead(this->sensor_pin);
    this->humidity = map(humidity_sensor_Value, this->sensor_wet, this->sensor_dry, 100, 0);
    humidity_difference = this->optimal_humidity - this->humidity;

    return humidity_difference;
};

void plant::watering(float humidity_difference, int water_amount) // water_amount is an example value
{
    if (humidity_difference > 0)
    {
        if (millis() - this->last_watered > 20000)
        {
            digitalWrite(this->motor_pin, HIGH);
            delay(100 * water_amount);
            digitalWrite(this->motor_pin, LOW);
            this->last_watered = millis();
            this->last_data_write = 0;
            this->watered = true;
            write_to_SDcard(this->SD_card_pin);
            //write_to_pc();
            this->watered = false;
        }
    }
};

// initialisation of the SD card I'll write the data to
void plant::write_to_SDcard(unsigned long measurment_frequency)
{
    /*static bool init is used to make sure that the file is only created once
    because it is static it will keep its value between function calls
    and will not be reinitialised every time the function is called*/
    static bool init = false;

    if (!SD.begin(this->SD_card_pin))
    {
        // error message
        // Serial.println(F("SD Card initialization failed!"));
        return;
    }

    file = SD.open((String(this->planttype) + "log.csv"), FILE_WRITE);
    // if the file is not initialised yet -> titles for csv file (if init == false)
    if (!init)
    {
        // add date to the file name so every time the program is started a new file is created
        // #ToDo

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
        }
        else
        {
            // error message
            // Serial.println(F("SD Card initialization failed!"));
            // Serial.print(F("SD Card: error on opening file.\n"));
        }
    }

    else
    {
        if (millis() - this->last_data_write < measurment_frequency)
        {
            return;
        }

        if (file)
        {
            // print the data to the file in csv format

            file.println((String(millis()/1000) + ", " + this->planttype + ", " + String(this->optimal_humidity) + ", " + String(this->humidity) + ", " + String(this->watered)));
            this->last_data_write = millis();
        }
        else
        {
            // error message
            // Serial.println(F("SD Card initialization failed!"));
            // Serial.print(F("SD Card: error on opening file. \n"));
        }
    }
    file.close();
}

void plant::write_to_pc(unsigned long measurment_frequency)
{
    static bool initiation = true;

    if (initiation)
    {

        Serial.println(F("runtime[s], planttype, optimal_humidity, humidity, watered"));
        initiation = false;
    }

    else
    {
        if (millis() - this->last_data_write < measurment_frequency)
        {
            return;
        }
        Serial.println();
        Serial.print(millis()/1000);
        Serial.print(F(", "));
        Serial.print(this->planttype );
        Serial.print(F(", "));
        Serial.print(this->optimal_humidity);
        Serial.print(F(", "));
        Serial.print(this->humidity);
        Serial.print(F(", "));
        Serial.print(this->watered);
        this->last_data_write = millis();
    }
}
