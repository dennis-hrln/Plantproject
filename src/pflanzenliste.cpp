
#include "pflanzenliste.h"

//stirps
const int moisturmeterPnin_stirps = A0;  // Moisture sensor pin A0 is an example value
const int motorPin_stirps = 7;           // Motor pin 8 is an example value
const int SD_card_pin_stirps = 4; // SD card pin 4 is an example value

plant Stirps("Stirps", 30.0f, moisturmeterPnin_stirps,
     motorPin_stirps, SD_card_pin_stirps);


//Kaktuss
plant Kaktus("Kaktus", 10.0f, A1, 13, 10);
