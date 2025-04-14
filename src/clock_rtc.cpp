
#include <Arduino.h>
#include <RTClib.h>
#include "clock_rtc.h"


bool starting_up(RTC_DS3231* rtc)
{
    if (!rtc->begin())
    {
        Serial.println(F("Couldn't find RTC"));
        return false;
    }
    if (rtc->lostPower())
    {
        // This will set the RTC to the date & time this sketch was compiled
        rtc->adjust(DateTime(F(__DATE__), F(__TIME__)));
        Serial.println(F("RTC lost power, setting the time!"));
        
    }
    Serial.println(F("RTC is running!"));
    
    return true;
}

void get_time(RTC_DS3231* rtc, TimeStruct* time, bool rtc_available)
{
    if (rtc_available == false)
    {
        time->year = 2004;
        time->month = 7;
        time->day = 2;
        time->hour = 23;
        time->minute = 59;
        time->second = 59;
        return;
    }
    else{
        DateTime now = rtc->now();   //create a DateTime object
        time->year = now.year();     //get the year
        time->month = now.month();   //get the month
        time->day = now.day();       //get the day
        time->hour = now.hour();     //get the hour
        time->minute = now.minute(); //get the minute
        time->second = now.second(); //get the second
    }
}

void adjust_year(RTC_DS3231* rtc)
{
    DateTime now = rtc->now();   //create a DateTime object
    short year = now.year() + 1;
    if (year > 2075)
    {
        year = 2025;
    }
    rtc->adjust(DateTime(year, now.month(), now.day(), now.hour(), now.minute(), now.second())); // Set the RTC to the new year
}

void adjust_month(RTC_DS3231* rtc)
{
    DateTime now = rtc->now();   //create a DateTime object
    byte month= now.month() + 1; // month gets bigger
    if (month > 12)
    {
        month = 1;
    }
    rtc->adjust(DateTime(now.year(), month, now.day(), now.hour(), now.minute(), now.second())); // Set the RTC to the new month
}

void adjust_day(RTC_DS3231* rtc)
{
    DateTime now = rtc->now();   //create a DateTime object
    byte day = now.day() + 1; // day gets bigger
    if (day > 31)
    {
        day = 1;
    }
    rtc->adjust(DateTime(now.year(), now.month(), day, now.hour(), now.minute(), now.second())); // Set the RTC to the new day
}

void adjust_hour(RTC_DS3231* rtc)
{
    DateTime now = rtc->now();   //create a DateTime object
    byte hour = now.hour() + 1; // hour gets bigger
    if (hour > 23)
    {
        hour = 0;
    }
    rtc->adjust(DateTime(now.year(), now.month(), now.day(), hour, now.minute(), now.second())); // Set the RTC to the new hour
}

void adjust_minute(RTC_DS3231* rtc)
{
    DateTime now = rtc->now();   //create a DateTime object
    byte minute = now.minute() + 1; // minute gets bigger
    if (minute > 59)
    {
        minute = 0;
    }
    rtc->adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), minute, now.second())); // Set the RTC to the new minute
}

void adjust_second(RTC_DS3231* rtc)
{
    DateTime now = rtc->now();   //create a DateTime object
    byte second = now.second() + 1; // second gets bigger
    if (second > 59)
    {
        second = 0;
    }
    rtc->adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), second)); // Set the RTC to the new second
}
