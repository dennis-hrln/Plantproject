#include "plant.h"

//Zimmerpflanze
const int moisturmeterPnin_zimmerpflanze = A5;  // Moisture sensor pin A0 is an example value
const int motorPin_zimmerpflanze = LED_BUILTIN;           // Motor pin 9 is an example value
const int SD_card_pin_zimmerpflanze = 4; // SD card pin 4 is an example value

plant Zimmerpflanze("Zimmerpflanze", 30.0f, moisturmeterPnin_zimmerpflanze,
     motorPin_zimmerpflanze, SD_card_pin_zimmerpflanze);


//Kaktuss
plant Kaktus("Kaktus", 10.0f, 12, 13, 10);

