#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <plant.h>
#include <screen.h>
#include "clock_rtc.h"
#include "pflanzenliste.h"
void next_button(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void next_button(TimeStruct water_time);
void select_button();

#endif // BUTTONS_H