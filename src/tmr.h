/**
 * AVR System Timer
 * @file    tmr.h
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 20:47:14
 * @edited  2015-04-28 00:00:18
 */

#ifndef TMR_H
#define TMR_H

#include <stdint.h>

/* Defines number of 8, 16, 32 bit timers
 * To save space, adjust to actual number of timers
 */
#define MAX_8_TIMERS    4
#define MAX_16_TIMERS   4
#define MAX_32_TIMERS   2

typedef enum {
    MSEC    =   1,
    SEC     =   2,
    MIN     =   3,
    HOUR    =   4,
    DAY     =   5
} sysT_scale;

typedef struct{
    uint8_t     active;
    uint8_t     msec;           //miliseconds counter
} sysTimer8_t;

typedef struct sysTimer16_t{
    sysT_scale  timer_scale;
    uint16_t    msec;
    uint16_t    sec;
    uint16_t    min;
    uint16_t    hour;
    uint16_t    day;
} sysTimer16_t;

typedef struct{
    sysT_scale  timer_scale;
    uint32_t    msec;
    uint32_t    sec;
    uint32_t    min;
    uint32_t    hour;
    uint32_t    day;
} sysTimer32_t;

/**
 * @brief Initialize timer hardware and timer arrays
 */
void sysT_init(void);

/* 8 bit timer functions */
void sysT_8_init(sysTimer8_t *timer);
void sysT_8_reset(sysTimer8_t *timer);
uint8_t sysT_8_get_msec(sysTimer8_t *timer);
/* 16 bit timer functions */
void sysT_16_init(sysTimer16_t *timer, sysT_scale scale);
void sysT_16_reset(sysTimer16_t *timer);
uint16_t sysT_16_get_msec(sysTimer16_t *timer);
uint16_t sysT_16_get_sec(sysTimer16_t *timer);
uint16_t sysT_16_get_min(sysTimer16_t *timer);
uint16_t sysT_16_get_hour(sysTimer16_t *timer);
uint16_t sysT_16_get_day(sysTimer16_t *timer);
/* 32 bit timer functions */
void sysT_32_init(sysTimer32_t *timer, sysT_scale scale);
void sysT_32_reset(sysTimer32_t *timer);

///**
// * @brief Create new counter
// * @param   timer_name  Name of new counter
// * @param   resolution  Scale of counter (MSEC, SEC, MIN, HOUR, DAY)
// * @return  success/failure of creating new counter
// */
//int tmr_new_counter(char *timer_name, sysT_scale resolution);
//
///**
// * @brief Get value of counter at it's predisignated scale
// * @param   timer_name  Name of counter
// */
//counter_int tmr_get_count(char *timer_name);
//
///**
// * @brief Get millisecond value of counter
// * @param   timer_name  Name of counter
// * Note, this will not give you the ellapsed time in milliseconds, unless the scale is MSEC.
// * Otherwise, it will return a value between 0 and 999. Once it overflows 1000,
// * it increments seconds and resets
// */
//counter_int tmr_get_msec(char *timer_name);
//
///**
// * @brief Get second value of counter
// * @param   timer_name  Name of counter
// * Note, this will not give you the ellapsed time in seconds, unless the scale is SEC.
// * Otherwise, it will return a value between 0 and 59. Once it overflows 60,
// * it increments minutes and resets
// * This value will remain 0 unless scale is SEC (seconds) or above
// */
//counter_int tmr_get_sec(char *timer_name);
//
///**
// * @brief Get minute value of counter
// * @param   timer_name  Name of counter
// * Note, this will not give you the ellapsed time in minutes, unless the scale is MIN.
// * Otherwise, it will return a value between 0 and 59. Once it overflows 60,
// * it increments hours and resets
// * This value will remain 0 unless sale is MIN (minutes) or above
// */
//counter_int tmr_get_min(char *timer_name);
//
///**
// * @brief Get hour value of counter
// * @param   timer_name  Name of counter
// * Note, this will not give you the ellapsed time in hours, unless the scale is HOUR.
// * Otherwise, it will return a value between 0 and 23. Once it overflows 24,
// * it increments days and resets
// * This value will remain 0 unless scale is HOUR or above
// */
//counter_int tmr_get_hour(char *timer_name);
//
///**
// * @brief Get day value of counter
// * @param   timer_name  Name of counter
// * This value will remain 0 unless scale is set to DAY
// */
//counter_int tmr_get_day(char *timer_name);
//
///**
// * @brief Resets timer to 0
// * @param   timer_name  Name of counter
// * Upon reset, counter will immediately start counting up again
// */
//void tmr_clear(char *timer_name);
//
#endif //TMR_H
