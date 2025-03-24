#include <Arduino.h>
int analogPin1 = 5;     //output pin
int analogPin2 = 6;     //output pin
int analogPin3 = 11;    //output pin
int analogpin = A0;     //input pin
float voltage = 1.5f;

void setup() {
    Serial.begin(9600);
    pinMode(analogPin1, OUTPUT);
    pinMode(analogPin2, OUTPUT);
    pinMode(analogPin3, OUTPUT);
    pinMode(analogpin, INPUT);
    
}

void loop() {
    // int potivalue = analogRead(A0);
    // voltage = map(potivalue, 0, 1023, 0, 8000);
    // map(voltage, 0, 8000, 0, 255);
    // if (voltage > 255)
    // {
    //     voltage = 200;
    // }
    Serial.println(String(voltage) + " realVoltage " + String((voltage/255)*5));

    analogWrite(analogPin1,(255*voltage)/5);
}
