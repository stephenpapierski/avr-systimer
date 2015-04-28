/**
 * AVR System Timer
 * @file    tmr.c
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 20:47:09
 * @edited  2015-04-28 01:04:31
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//TESTING
#include <stdio.h>
#include "avr-uart/uart.h"
//

#include "tmr.h"

/******************************************************************************/
/* Declarations                                                               */
/******************************************************************************/

//These keep track of what timers have been initialized
sysTimer8_t *timers8[MAX_8_TIMERS];
sysTimer16_t *timers16[MAX_16_TIMERS];
//sysTimer32_t *timers32[MAX_32_TIMERS];

//keeps track of next available timer slot
uint8_t next_timer8;
uint8_t next_timer16;
//uint8_t next_timer32;

/******************************************************************************/
/* Local Utilities                                                            */
/******************************************************************************/

/* 8 bit timer utils */

void update_timers8(void){
    uint8_t i;
    for (i = 0; i < MAX_8_TIMERS; i++){
        if (timers8[i] -> active){
            timers8[i] -> ticks++;
        }
    }
}

/* 16 bit timer utils */

void update_timers16(void){
    uint8_t i;
    for (i = 0; i < MAX_16_TIMERS; i++){
        if (timers16[i] -> active){
            timers16[i] -> ticks++;
        }
    }
}


///* 32 bit timer utils */
//
//void update_timers32(void){
//    uint8_t i;
//    for (i = 0; i < MAX_32_TIMERS; i++){
//        if (timers32[i] -> active){
//            timers32[i] -> ticks++;
//        }
//    }
//}

/******************************************************************************/
/* Library Functions                                                          */
/******************************************************************************/

void sysT_init(void){
    //hardware setup
#if defined (__AVR_ATtiny85__)
    TCCR0A = 1<<WGM01;  //sets Timer0 to CTC mode
    TCCR0B = 1<<CS01;   //sets Timer0 @ (1MHz/8)
    OCR0A = 125;        //set Timer0 to CompA every 1 ms
    TIMSK = 1<<OCIE0A;  //enables Timer0 Compare A Interrupt
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
    TCCR0A = 1<<WGM01;  //sets Timer0 to CTC mode
    TCCR0B = 1<<CS01 | 1<<CS00;   //sets Timer0 @ (16MHz/8)
    OCR0A = 250;        //set Timer0 to CompA every 1ms
    TIMSK0 = 1<<OCIE0A;  //enables Timer0 Compare A Interrupt
#elif defined (__AVR_ATmega1284P__)
    //insert timer regs for 1284p
#endif
}

void sysT_timer_service(void){
    update_timers8();
    update_timers16();
    //update_timers32();
}

/* 8 bit timer functions */

void sysT_8_init(sysTimer8_t *timer){
    sysT_8_reset(timer);
    timers8[next_timer8] = timer;
    timer -> active = true;
    next_timer8++;
}

void sysT_8_reset(sysTimer8_t *timer){
    (timer -> ticks) = 0;
}

uint8_t sysT_8_get_ticks(sysTimer8_t *timer){
    uint8_t ret;
    cli();
    ret = timer->ticks;
    sei();
    return ret;
}

/* 16 bit timer functions */

void sysT_16_init(sysTimer16_t *timer){
    sysT_16_reset(timer);
    timers16[next_timer16] = timer;
    timer -> active = true;
    next_timer16++;
}

void sysT_16_reset(sysTimer16_t *timer){
    (timer -> ticks) = 0;
}

uint16_t sysT_16_get_ticks(sysTimer16_t *timer){
    uint16_t ret;
    cli();
    ret = timer->ticks;
    sei();
    return ret;
}

///* 32 bit timer functions */
//
//void sysT_32_init(sysTimer32_t *timer){
//    sysT_32_reset(timer);
//    timers32[next_timer32] = timer;
//    timer -> active = true;
//    next_timer32++;
//}
//
//void sysT_32_reset(sysTimer32_t *timer){
//    (timer -> ticks) = 0;
//}
//
//uint32_t sysT_32_get_ticks(sysTimer32_t *timer){
//    uint32_t ret;
//    cli();
//    ret = timer->ticks;
//    sei();
//    return ret;
//}

/* ISR */
ISR(TIMER0_COMPA_vect){
    cli();
    sysT_timer_service();
    sei();
}
