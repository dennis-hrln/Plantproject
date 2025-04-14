#ifndef CLOCK_RTC_CPP_H
#define CLOCK_RTC_CPP_H
struct TimeStruct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};
bool starting_up(RTC_DS3231* rtc);
void get_time(RTC_DS3231* rtc, TimeStruct* time_struct, bool rtc_available);
void adjust_year(RTC_DS3231* rtc);
void adjust_month(RTC_DS3231* rtc);
void adjust_day(RTC_DS3231* rtc);
void adjust_hour(RTC_DS3231* rtc);
void adjust_minute(RTC_DS3231* rtc);
void adjust_second(RTC_DS3231* rtc);


#endif // CLOCK_RTC_CPP_H