#include <RTClib.h>
#include <Wire.h>
RTC_DS3231 rtc;



void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(3000);
  //rtc.begin();
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
       // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }
}

void loop() {
    Serial.print("Current Date & Time: ");
//   DateTime now = rtc.now();
//   Serial.println("Current Date & Time: ");
//     Serial.print(now.year(), DEC);


}