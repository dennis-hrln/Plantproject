#ifndef CLOCK_RTC_CPP_H
#define CLOCK_RTC_CPP_H
#include <Arduino.h>
#include <RTClib.h>
#include <Ds1302.h>
struct TimeStruct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};
bool starting_up(RTC_DS3231* rtc);
bool starting_up(Ds1302* rtc);
void reset_time(Ds1302* rtc);
void get_time(RTC_DS3231* rtc, TimeStruct* time_struct, bool rtc_available);
void get_time(Ds1302* rtc, TimeStruct* time_struct, bool rtc_available);
void adjust_year(RTC_DS3231* rtc);
void adjust_year(Ds1302* rtc);
void adjust_month(RTC_DS3231* rtc);
void adjust_month(Ds1302* rtc);
void adjust_day(RTC_DS3231* rtc);
void adjust_day(Ds1302* rtc);
void adjust_hour(RTC_DS3231* rtc);
void adjust_hour(Ds1302* rtc);
void adjust_minute(RTC_DS3231* rtc);
void adjust_minute(Ds1302* rtc);
void adjust_second(RTC_DS3231* rtc);
void adjust_second(Ds1302* rtc);


/* Functions where I can set the rtc as a random val. Works as if the rtc doesn't exist*/
bool starting_up(byte* rtc);
void reset_time(byte* rtc);
void get_time(byte* rtc, TimeStruct* time_struct, bool rtc_available);
void adjust_year(byte* rtc);
void adjust_month(byte* rtc);
void adjust_day(byte* rtc);
void adjust_hour(byte* rtc);
void adjust_minute(byte* rtc);
void adjust_second(byte* rtc);


#endif // CLOCK_RTC_CPP_H