#ifndef CLOCK_RTC_CPP_H
#define CLOCK_RTC_CPP_H

bool starting_up(RTC_DS3231* rtc);
short adjust_year(RTC_DS3231* rtc);
void adjust_month(RTC_DS3231* rtc);
void adjust_day(RTC_DS3231* rtc);
void adjust_hour(RTC_DS3231* rtc);
void adjust_minute(RTC_DS3231* rtc);
void adjust_second(RTC_DS3231* rtc);

#endif // CLOCK_RTC_CPP_H