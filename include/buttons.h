#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <plant.h>
#include <screen.h>
#include "clock_rtc.h"
#include "pflanzenliste.h"
void next_button();
void select_button(plant *, int);

#endif // BUTTONS_H