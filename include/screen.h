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
    void water_disp(unsigned long);      // Display the last time the plant was watered
    void calibration_disp();
    unsigned long stay_awake_time = 60 * 1000;  // in ms
    void screen_dimming();  // Turns off the backlight after a certain time
    void innit();
    void update_screen(String plantname, int humidity, int optimal_humidity, unsigned long last_watered);
    unsigned long last_disp_action = 0;
    String disp_status;
    unsigned long last_disp_change = 0;
    bool lit = true;

    private:
    //Custom characters
    
    // byte right_arrow[8] = {
    //     0b00000,
    //     0b00000,
    //     0b00000,
    //     0b00010,
    //     0b11111,
    //     0b00010,
    //     0b00000,
    //     0b00000
    // };
    // byte down_arrow[8] = {
    //     0b00100,
    //     0b00100,
    //     0b00100,
    //     0b00100,
    //     0b00100,
    //     0b11111,
    //     0b01110,
    //     0b00100
    // };
    // byte up_arrow[8] = {
    //     0b00100,
    //     0b01110,
    //     0b11111,
    //     0b00100,
    //     0b00100,
    //     0b00100,
    //     0b00100,
    //     0b00100
    // };
    // byte left_arrow[8] = {
    //     0b00000,
    //     0b00000,
    //     0b00000,
    //     0b01000,
    //     0b11111,
    //     0b01000,
    //     0b00000,
    //     0b00000
    // };
    byte select_char[8] = {
        B00000,
        B00100,
        B01110,
        B11111,
        B11111,
        B01110,
        B00100,
        B00000
      };
    
};

#endif // SCREEN_H