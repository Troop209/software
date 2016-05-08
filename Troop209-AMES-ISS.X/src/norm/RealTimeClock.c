
#include "system.h"
// #include "Real Time Clock.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - 
// -                Structures
// -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define getDaysInMonth(m,y) ((m == 2) && !(y & 0b11)? 29: daysInMonth[m])
static const unsigned daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

typedef enum {
    SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
} Weekday;

typedef enum {
    JANUARY = 1, FEBUARY, MARCH, APRIL, MAY, JUNE,
    JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
} Month;

typedef struct {
    signed seconds;
    signed tensOfSeconds;
    signed minutes;
    signed tensOfMinutes;
    signed hours;
    signed tensOfHours;
} Clock;

typedef struct {
    signed weekday;
    signed days;
    signed tensOfDays;
    signed months;
    signed tensOfMonths;
    signed years;
    signed tensOfYears;
} Calendar;


typedef struct {
    union {
        Clock clock;
        struct {
            signed seconds;
            signed tensOfSeconds;
            signed minutes;
            signed tensOfMinutes;
            signed hours;
            signed tensOfHours;
        };
    };
    union {
        Calendar calendar;
        struct {
            signed weekday;
            signed days;
            signed tensOfDays;
            signed months;
            signed tensOfMonths;
            signed years;
            signed tensOfYears;
        };
    };
} CalendarAndClock;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - 
// -                Function Prototypes:
// -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CalendarAndClock timeStampToCalendarAndClock(String timeStamp)  ;
CalendarAndClock numbersToCalendarAndClock(       int tensOfYears,    int years,
                                                  int tensOfMonths,   int months,
                                                  int tensOfDays,     int days,
                                                  int tensOfHours,    int hours,
                                                  int tensOfMinutes,  int minutes,
                                                  int tensOfSeconds,  int seconds)  ;
CalendarAndClock newCalendarAndClock(void)  ;
int              calculateWeekday(unsigned year, unsigned month, unsigned day)   ;
int              I2c2_SetRTCString(CalendarAndClock temp)   ;
int     i2c2_TalkToDevice(char addr, char I2C2_slen, 
        char* I2C2_sbuf, char I2C2_rlen, char* I2C2_rbuf) ;
 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - 
// -                Variables:
// -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    char    RTC_NEW_TIME[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;
   

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - 
// -                Functions:
// -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


CalendarAndClock timeStampToCalendarAndClock(String timeStamp)
{
    /* use the numbers to Calendar Clock function to simplify the passed stamp, and then return it */
    return (CalendarAndClock) numbersToCalendarAndClock(timeStamp[0]  - '0', timeStamp[1]  - '0',
                                     timeStamp[3]  - '0', timeStamp[4]  - '0',
                                     timeStamp[6]  - '0', timeStamp[7]  - '0',
                                     timeStamp[9]  - '0', timeStamp[10] - '0',
                                     timeStamp[12] - '0', timeStamp[13] - '0',
                                     timeStamp[15] - '0', timeStamp[16] - '0');
}// simplifyDate end

CalendarAndClock numbersToCalendarAndClock(       int tensOfYears,    int years,
                                                  int tensOfMonths,   int months,
                                                  int tensOfDays,     int days,
                                                  int tensOfHours,    int hours,
                                                  int tensOfMinutes,  int minutes,
                                                  int tensOfSeconds,  int seconds)
{
    /* variable used to store values temporarily during calculations */
    int extra, tempHours, tempDays, tempMonths, tempYears;

    if (seconds > 9)
    {
        extra = seconds / 10;
        tensOfSeconds += extra;
        seconds = seconds - extra * 10;
    }
    else if (seconds < 0)
    {
        extra = seconds / 10 - 1;
        tensOfSeconds += extra;
        seconds = seconds - extra * 10;
    }

    if (tensOfSeconds > 5)
    {
        extra = tensOfSeconds / 6;
        minutes += extra;
        tensOfSeconds = tensOfSeconds - extra * 6;
    }
    else if (tensOfSeconds < 0)
    {
        extra = tensOfSeconds / 6 - 1;
        minutes += extra;
        tensOfSeconds = tensOfSeconds - extra * 6;
    }

    if (minutes > 9)
    {
        extra = minutes / 10;
        tensOfMinutes += extra;
        minutes = minutes - extra * 10;
    }
    else if (minutes < 0)
    {
        extra = minutes / 10 - 1;
        tensOfMinutes += extra;
        minutes = minutes - extra * 10;
    }

    if (tensOfMinutes > 5)
    {
        extra = tensOfMinutes / 6;
        hours += extra;
        tensOfMinutes = tensOfMinutes - extra * 6;
    }
    else if (tensOfMinutes < 0)
    {
        extra = tensOfMinutes / 6 - 1;
        hours += extra;
        tensOfMinutes = tensOfMinutes - extra * 6;
    }

    tempHours = hours + tensOfHours * 10;
    if (tempHours >= 0)
    {
        extra = tempHours / 24;
        days += extra;
        extra = tempHours - extra * 24;
        tensOfHours = extra / 10;
        hours = extra - tensOfHours * 10;
    }
    else if (tempHours < 0)
    {
        extra = tempHours / 24 - 1;
        days += extra;
        extra = tempHours - extra * 24;
        tensOfHours = extra / 10;
        hours = extra - tensOfHours * 10;
    }

    tempMonths = months + tensOfMonths * 10;
    if (tempMonths > 12)
    {
        extra = tempMonths / 12;
        years += extra;
        tempMonths = tempMonths - extra * 12;
    }
    else if (tempMonths <= 0)
    {
        extra = tempMonths / 12 - 1;
        years += extra;
        tempMonths = tempMonths - extra * 12;
    }

    if (years > 9)
    {
        extra = years / 10;
        tensOfYears += extra;
        years = years - extra * 10;
    }
    else if (years < 0)
    {
        extra = years / 10 - 1;
        tensOfYears += extra;
        years = years - extra * 10;
    }

    if (tensOfYears > 9)
    {
        extra = tensOfYears / 10;
        tensOfYears = tensOfYears - extra * 10;
    }
    else if (tensOfYears < 0)
    {
        extra = tensOfYears / 10 - 1;
        tensOfYears = tensOfYears - extra * 10;
    }

    tempYears = years + tensOfYears * 10 + 100;

    tempDays = days + tensOfDays * 10;
    if (tempDays > 1460)
    {
        extra = tempDays / 1461;
        tempYears += extra << 2;
        tempDays = tempDays - extra * 1461;
    }
    else if (tempDays < 0)
    {
        extra = tempDays / 1461 - 1;
        tempYears += extra * 4;
        tempDays = tempDays - extra * 1461;
    }

    while (tempDays > 366)
    {
        extra = tempYears & 0b11;
        if (extra == 3 && tempMonths > FEBUARY)
            tempDays -= 366;
        else if (extra == 0 && tempMonths < MARCH)
            tempDays -= 366;
        else
            tempDays -= 365;
        ++tempYears;

    }

    while (tempDays <= 0)
    {
        if (tempYears == 0)
            tempYears = 99;
        else
            --tempYears;

        extra = tempYears & 0b11;

        if (extra == 3 && tempMonths > FEBUARY)
            tempDays += 366;
        else if (extra == 0 && tempMonths < MARCH)
            tempDays += 366;
        else
            tempDays += 365;
    }

    while (tempDays > getDaysInMonth(tempMonths,tempYears))
    {
        tempDays -= getDaysInMonth(tempMonths,tempYears);
        ++tempMonths;

        if (tempMonths > 12)
        {
            tempMonths -= 12;
            ++tempYears;
        }
   }

    tensOfDays = tempDays / 10;
    days = tempDays - tensOfDays * 10;

    if (tempMonths > 9)
    {
        tensOfMonths = 1;
        months = tempMonths - 10;
    }
    else
    {
        tensOfMonths = 0;
        months = tempMonths;
    }

    tempYears %= 100;
    tensOfYears = tempYears / 10;
    years = tempYears - tensOfYears * 10;
    int weekday = calculateWeekday(tempYears, tempMonths, tempDays);

    CalendarAndClock temp = newCalendarAndClock();
    temp.seconds = seconds;
    temp.tensOfSeconds = tensOfSeconds;
    temp.minutes = minutes;
    temp.tensOfMinutes = tensOfMinutes;
    temp.hours = hours;
    temp.tensOfHours = tensOfHours;
    temp.weekday = weekday;
    temp.days = days;
    temp.tensOfDays = tensOfDays;
    temp.months = months;
    temp.tensOfMonths = tensOfMonths;
    temp.years = years;
    temp.tensOfYears = tensOfYears;

    return (CalendarAndClock) temp;
}

CalendarAndClock newCalendarAndClock(void) {
    static CalendarAndClock temp = {
        {
            {0}},
        {
            {.months = JANUARY, .weekday = SATURDAY, .days = 1}}};
    return temp;
}

int calculateWeekday(unsigned year, unsigned month, unsigned day)
{
    /**
     * Weekday Algorithm
     *
     * The following algorithm is used to calculate the weekday:
     *  1) Look up century number (it is '6' for 2000 - 2099).
     *  2) Note the last two digits of the year.
     *  3) Integer divide the last two digits of the year by 4.
     *  4) Look up the month number in the month table.
     *     a) During leap years, subtract 1 from January's and February's number
     *  5) Add the numbers from steps 1-4 to the day of the month.
     *  6) Modulo 7 the result of step 5 (this is the day of the week).
     */
    static const int monthsTable[] = {0,3,3,6,1,4,6,2,5,0,3,5};

    if((year & 0b11) || (month < 3))
        return (6 + year + (year >> 2) + monthsTable[month - 1] + day) % 7;
    else
        return (6 + year + (year >> 2) + monthsTable[month - 1] + day - 1) % 7;

}

int I2c2_SetRTCString(CalendarAndClock temp)
{   int stat    = 0;
    RTC_NEW_TIME[0] = 0 ;   // Set the RTC  address
    RTC_NEW_TIME[1] = temp.tensOfSeconds *16 + temp.seconds   ;
    RTC_NEW_TIME[2] = temp.tensOfMinutes *16 + temp.minutes   ;
    RTC_NEW_TIME[3] = temp.tensOfHours   *16 + temp.hours     ;
    RTC_NEW_TIME[4] = temp.weekday    ;
    RTC_NEW_TIME[5] = temp.tensOfDays    *16 + temp.days      ;
    RTC_NEW_TIME[6] = temp.tensOfMonths  *16 + temp.months    ;
    RTC_NEW_TIME[7] = temp.tensOfYears   *16 + temp.years     ;
    stat=i2c2_TalkToDevice(0x68, 8, RTC_NEW_TIME,  0, NullPtr) ;
    return (stat)   ;
}