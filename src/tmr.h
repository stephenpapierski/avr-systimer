/**
 * AVR System Timer
 * @file    tmr.h
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 20:47:14
 * @edited  2015-04-24 14:35:29
 */

#ifndef TMR_H
#define TMR_H

//number of counters
#define NUM_USER_COUNTERS            4
//resolution of counters
#define COUNTER_INT_TYPE        uint16_t
//max length of names for counters
#define COUNTER_NAME_MAX_LENGTH 16
//comment out if you don't want a system counter that counts msec:sec:min:hour:day
#define __SYS_COUNTER__

//TODO idea use typedef enum to create timer names
//as well as structs to hold an entire timer

typedef COUNTER_INT_TYPE counter_int;

typedef enum {
    MSEC,
    SEC,
    MIN,
    HOUR,
    DAY,
    ALL
} EResolution;

/**
 * @brief Initialize timer hardware and timer arrays
 */
void tmr_init(void);

/**
 * @brief Create new counter
 * @param   timer_name  Name of new counter
 * @param   resolution  Scale of counter (MSEC, SEC, MIN, HOUR, DAY)
 * @return  success/failure of creating new counter
 */
int tmr_new_counter(char *timer_name, EResolution resolution);

/**
 * @brief Get value of counter at it's predisignated scale
 * @param   timer_name  Name of counter
 */
counter_int tmr_get_count(char *timer_name);

/**
 * @brief Get millisecond value of counter
 * @param   timer_name  Name of counter
 * Note, this will not give you the ellapsed time in milliseconds, unless the scale is MSEC.
 * Otherwise, it will return a value between 0 and 999. Once it overflows 1000,
 * it increments seconds and resets
 */
counter_int tmr_get_msec(char *timer_name);

/**
 * @brief Get second value of counter
 * @param   timer_name  Name of counter
 * Note, this will not give you the ellapsed time in seconds, unless the scale is SEC.
 * Otherwise, it will return a value between 0 and 59. Once it overflows 60,
 * it increments minutes and resets
 * This value will remain 0 unless scale is SEC (seconds) or above
 */
counter_int tmr_get_sec(char *timer_name);

/**
 * @brief Get minute value of counter
 * @param   timer_name  Name of counter
 * Note, this will not give you the ellapsed time in minutes, unless the scale is MIN.
 * Otherwise, it will return a value between 0 and 59. Once it overflows 60,
 * it increments hours and resets
 * This value will remain 0 unless sale is MIN (minutes) or above
 */
counter_int tmr_get_min(char *timer_name);

/**
 * @brief Get hour value of counter
 * @param   timer_name  Name of counter
 * Note, this will not give you the ellapsed time in hours, unless the scale is HOUR.
 * Otherwise, it will return a value between 0 and 23. Once it overflows 24,
 * it increments days and resets
 * This value will remain 0 unless scale is HOUR or above
 */
counter_int tmr_get_hour(char *timer_name);

/**
 * @brief Get day value of counter
 * @param   timer_name  Name of counter
 * This value will remain 0 unless scale is set to DAY
 */
counter_int tmr_get_day(char *timer_name);

/**
 * @brief Resets timer to 0
 * @param   timer_name  Name of counter
 * Upon reset, counter will immediately start counting up again
 */
void tmr_clear(char *timer_name);

#endif //TMR_H
