/*

# date;date -u
Wed Sep 16 14:14:46 CST 2015
Wed Sep 16 06:14:46 UTC 2015
功能： 将utc时间转换为本地cst()时间
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#if 0
typedef long __kernel_suseconds_t;
typedef __kernel_suseconds_t suseconds_t;

typedef struct timeval {
        time_t tv_sec; /* seconds */
        suseconds_t tv_usec; /* microseconds */
}TIMEVAL;

typedef struct timespec {
        time_t tv_sec; /* seconds */
        long tv_nsec; /* nanoseconds */
}TIMESPEC;

typedef struct timezone {
        int tz_minuteswest; /* minutes west of Greenwich */
        int tz_dsttime; /* type of dst correction */
}TIMEZONE;
#endif
/*
 * * The struct used to pass data via the following ioctl. Similar to the
 * * struct tm in <time.h>, but it needs to be here so that the kernel
 * * source is self contained, allowing cross-compiles, etc. etc.
 * */

typedef struct rtc_time {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
}RTC_TIME;

static const unsigned char rtc_days_in_month[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static const unsigned short rtc_ydays[2][13] = {
        /* Normal years */
        { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
        /* Leap years */
        { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};

#define LEAPS_THRU_END_OF(y) ((y)/4 - (y)/100 + (y)/400)
#define LEAP_YEAR(year) ((!(year % 4) && (year % 100)) || !(year % 400))

/*
 * The number of days in the month.
 */
int rtc_month_days(unsigned int month, unsigned int year)
{
        return rtc_days_in_month[month] + (LEAP_YEAR(year) && month == 1);
}

/*
 * The number of days since January 1. (0 to 365)
 */
int rtc_year_days(unsigned int day, unsigned int month, unsigned int year)
{
        return rtc_ydays[LEAP_YEAR(year)][month] + day-1;
}

/*
 * Convert seconds since 01-01-1970 00:00:00 to Gregorian date.
 */
void rtc_time_to_tm(unsigned long time, struct rtc_time *tm)
{
        register int days, month, year;

        days = time / 86400;
        time -= days * 86400;

        /* day of the week, 1970-01-01 was a Thursday */
        tm->tm_wday = (days + 4) % 7;

        year = 1970 + days / 365;
        days -= (year - 1970) * 365
                + LEAPS_THRU_END_OF(year - 1)
                - LEAPS_THRU_END_OF(1970 - 1);
        if (days < 0) {
                year -= 1;
                days += 365 + LEAP_YEAR(year);
        }
        tm->tm_year = year - 1900;
        tm->tm_yday = days + 1;

        for (month = 0; month < 11; month++) {
                int newdays;

                newdays = days - rtc_month_days(month, year);
                if (newdays < 0)
                        break;
                days = newdays;
        }
        tm->tm_mon = month;
        tm->tm_mday = days + 1;

        tm->tm_hour = time / 3600;
        time -= tm->tm_hour * 3600;
        tm->tm_min = time / 60;
        tm->tm_sec = time - tm->tm_min * 60;
}

int main(void){

        struct timeval tv;
        struct timezone tz;
        RTC_TIME tm;
        gettimeofday(&tv, &tz);

        rtc_time_to_tm(tv.tv_sec, &tm);
        fprintf(stdout, "UTC time :%04d-%02d-%02d %02d:%02d:%02d \n",
                        tm.tm_year+1900,tm.tm_mon, tm.tm_mday, tm.tm_hour,
                        tm.tm_min, tm.tm_sec);

        rtc_time_to_tm(tv.tv_sec + 60*60*8, &tm);
        fprintf(stdout, "After adjust :%04d-%02d-%02d %02d:%02d:%02d \n",
                        tm.tm_year+1900,tm.tm_mon, tm.tm_mday, tm.tm_hour,
                        tm.tm_min, tm.tm_sec);

        return 0;

}


