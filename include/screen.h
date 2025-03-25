#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


class screen
{

    public:
    screen(LiquidCrystal_I2C* LCD);
    LiquidCrystal_I2C* lcd;
    
    void home_disp(String plantname, int humidity, int optimal_humidity);   // Display humidity /optimal humdiity
    void water_disp();      // Display the last time the plant was watered
    void calibration_disp();
    unsigned long stay_awake_time = 40 * 1000;  // 40 seconds
    void screen_dimming();  // Turns off the backlight after a certain time
    void innit();
    unsigned long last_disp_action = 0;
    String disp_status;
    //todo if buttin pushed : last_disp_activation = millis();
    //disp.home
    // - add to every button function

    private:
    //Custom characters
    
    byte right_arrow[8] = {
        0b00000,
        0b00000,
        0b00000,
        0b00010,
        0b11111,
        0b00010,
        0b00000,
        0b00000
    };
    byte down_arrow[8] = {
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b11111,
        0b01110,
        0b00100
    };
    byte up_arrow[8] = {
        0b00100,
        0b01110,
        0b11111,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100
    };
    byte left_arrow[8] = {
        0b00000,
        0b00000,
        0b00000,
        0b01000,
        0b11111,
        0b01000,
        0b00000,
        0b00000
    };

    unsigned long last_disp_change = 0;
};

#endif // SCREEN_H