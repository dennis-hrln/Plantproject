#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


class screen
{

    public:
    screen(LiquidCrystal_I2C* LCD);
    void innit();
    void home_disp();
    void water_disp();
    void calibration_disp();
    unsigned long stay_awake_time = 25 * 1000;  // 15 seconds


    private:
    //Custom characters
    void screen_dimming();
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