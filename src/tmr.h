/**
 * AVR System Timer
 * @file    tmr.h
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 20:47:14
 * @edited  2015-04-28 01:04:01
 */

#ifndef TMR_H
#define TMR_H

#include <stdint.h>
#include <stdbool.h>

/* Defines number of 8, 16, 32 bit timers
 * To save space, adjust to actual number of timers
 */
#define MAX_8_TIMERS    4
#define MAX_16_TIMERS   4
//#define MAX_32_TIMERS   2

typedef struct sysTimer8_t{
    bool        active;
    uint8_t     ticks;
} sysTimer8_t;

typedef struct sysTimer16_t{
    bool        active;
    uint16_t    ticks;
} sysTimer16_t;

//typedef struct sysTimer32_t{
//    bool        active;
//    uint32_t    ticks;
//} sysTimer32_t;

/**
 * @brief Initialize timer hardware and timer arrays
 */
void sysT_init(void);

/* 8 bit timer functions */
void sysT_8_init(sysTimer8_t *timer);
void sysT_8_reset(sysTimer8_t *timer);
uint8_t sysT_8_get_ticks(sysTimer8_t *timer);

/* 16 bit timer functions */
void sysT_16_init(sysTimer16_t *timer);
void sysT_16_reset(sysTimer16_t *timer);
uint16_t sysT_16_get_ticks(sysTimer16_t *timer);

///* 32 bit timer functions */
//void sysT_32_init(sysTimer32_t *timer);
//void sysT_32_reset(sysTimer32_t *timer);
//uint32_t sysT_32_get_ticks(sysTimer32_t *timer);

#endif //TMR_H
