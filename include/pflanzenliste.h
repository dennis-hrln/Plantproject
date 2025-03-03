#include "plant.h"

//Zimmerpflanze
const int moisturmeterPnin_zimmerpflanze = A0;  // Moisture sensor pin A0 is an example value
const int motorPin_zimmerpflanze = LED_BUILTIN;           // Motor pin 9 is an example value
plant Zimmerpflanze("Zimmerpflanze", 20.0f, moisturmeterPnin_zimmerpflanze, motorPin_zimmerpflanze);


//Kaktuss
plant Kaktus("Kaktus", 10.0f, A1,LED_BUILTIN);
