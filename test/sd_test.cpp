#include <Arduino.h>
#include "sd_card.h"

const int chipSelect = 10; // CS pin for SD card

void setup() {
    Serial.begin(9600);

    Serial.println("Initializing SD card...");
    pinMode(chipSelect, OUTPUT);
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card initialization failed!");
        return;
    }
    Serial.println("SD card initialized.");
    File testFile = SD.open("11______.csv", FILE_WRITE);
    if (testFile) {
        testFile.println("SD card test successful!");
        testFile.close();
        Serial.println("Wrote to test.txt");

    File test = SD.open("12_______.txt", FILE_WRITE);
    }
}

void loop() {
    // nothing to do here
}
