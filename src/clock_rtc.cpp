
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




bool starting_up(Ds1302* rtc)
{   
    
    rtc->init();
    // test if clock is halted and set a date-time (see example 2) to start it
    if (rtc->isHalted())
    {
        Serial.println("RTC is halted. Setting time...");

        Ds1302::DateTime dt = {
            .year = 25,
            .month = 11,
            .day = 11,
            .hour = 11,
            .minute = 11,
            .second = 11,
            // .dow = calculateDayOfWeek(.year, .month, .day);
            .dow = 1 // 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
        };

        rtc->setDateTime(&dt);
        rtc-> start();

        return true; // RTC was halted and time was set
    }
    return true;
}

void reset_time(Ds1302* rtc)
{
    // Serial.println("reset_time() called");
    Ds1302::DateTime dt = {
        .year = 25,
        .month = 01,
        .day = 01,
        .hour = 11,
        .minute = 59,
        .second = 10,
        .dow = 1 // 1 = Monday, 2 = Tuesday, ..., 7 = Sunday
    };
    rtc->setDateTime(&dt);
}

void get_time(Ds1302* rtc, TimeStruct* time, bool rtc_available)
{
    if (rtc_available == false)
    {
        time->year = 04;
        time->month = 07;
        time->day = 02;
        time->hour = 23;
        time->minute = 59;
        time->second = 59;
        return;
    }
    else{
        Ds1302::DateTime now;
        rtc->getDateTime(&now);
        time->year = now.year;     //get the year
        time->month = now.month;   //get the month
        time->day = now.day;       //get the day
        time->hour = now.hour;     //get the hour
        time->minute = now.minute; //get the minute
        time->second = now.second; //get the second
    }
}

void adjust_year(Ds1302* rtc)
{
    Ds1302::DateTime now;       //create a DateTime object 
    rtc->getDateTime(&now);
    short year = now.year + 1;
    if (year > 75)
    {
        year = 25;
    }
    Ds1302::DateTime newTime = {year, now.month, now.day, now.hour, now.minute, now.second, now.dow};
    rtc->setDateTime(&newTime); // Set the RTC to the new year
}

void adjust_month(Ds1302* rtc)
{
    Ds1302::DateTime now;       //create a DateTime object 
    rtc->getDateTime(&now);
    byte month= now.month + 1; // month gets bigger
    if (month > 12)
    {
        month = 1;
    }
    Ds1302::DateTime newTime = {now.year, month, now.day, now.hour, now.minute, now.second, now.dow};
    rtc->setDateTime(&newTime); // Set the RTC to the new month
}

void adjust_day(Ds1302* rtc)
{
    Ds1302::DateTime now;       //create a DateTime object 
    rtc->getDateTime(&now);
    byte day = now.day + 1; // day gets bigger
    if (day > 31)
    {
        day = 1;
    }
    Ds1302::DateTime newTime = {now.year, now.month, day, now.hour, now.minute, now.second, now.dow};
    rtc->setDateTime(&newTime); // Set the RTC to the new day
}

void adjust_hour(Ds1302* rtc)
{
    Ds1302::DateTime now;       //create a DateTime object 
    rtc->getDateTime(&now);
    byte hour = now.hour + 1; // hour gets bigger
    if (hour > 23)
    {
        hour = 0;
    }
    Ds1302::DateTime newTime = {now.year, now.month, now.day, hour, now.minute, now.second, now.dow};
    rtc->setDateTime(&newTime); // Set the RTC to the new hour
}

void adjust_minute(Ds1302* rtc)
{
    Ds1302::DateTime now;       //create a DateTime object 
    rtc->getDateTime(&now);
    byte minute = now.minute + 1; // minute gets bigger
    if (minute > 59)
    {
        minute = 0;
    }
    Ds1302::DateTime newTime = {now.year, now.month, now.day, now.hour, minute, now.second, now.dow};
    rtc->setDateTime(&newTime); // Set the RTC to the new minute
}

void adjust_second(Ds1302* rtc)
{
    Ds1302::DateTime now;       //create a DateTime object 
    rtc->getDateTime(&now);
    byte second = now.second + 1; // second gets bigger
    if (second > 59)
    {
        second = 0;
    Ds1302::DateTime newTime = {now.year, now.month, now.day, now.hour, now.minute, second, now.dow};
    rtc->setDateTime(&newTime); // Set the RTC to the new second
}
}

bool starting_up(byte* rtc)
{
    return false;
}
void reset_time(byte* rtc)
{
    return;
}
void get_time(byte* rtc, TimeStruct* time_struct, bool rtc_available)
{
    return;
}
void adjust_year(byte* rtc)
{
    return;
}
void adjust_month(byte* rtc)
{
    return;
}
void adjust_day(byte* rtc)
{
    return;
}
void adjust_hour(byte* rtc)
{
    return;
}
void adjust_minute(byte* rtc)
{
    return;
}
void adjust_second(byte* rtc)
{
    return;
}