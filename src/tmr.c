/**
 * AVR System Timer
 * @file    tmr.c
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 20:47:09
 * @edited  2015-04-25 01:36:50
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
sysTimer32_t *timers32[MAX_32_TIMERS];

//keeps track of next available timer slot
uint8_t next_timer8;
uint8_t next_timer16;
uint8_t next_timer32;

/******************************************************************************/
/* Local Utilities                                                            */
/******************************************************************************/

/* 8 bit timer utils */

void update_timers8(void){
    uint8_t i;
    for (i = 0; i < MAX_8_TIMERS; i++){
        if (timers8[i] -> active){
            timers8[i] -> msec++;
        }
    }
}

/* 16 bit timer utils */

void update_sec16(uint16_t index){
    if ((timers16[index] -> msec) > 999){
        (timers16[index] -> msec) = 0;
      timers16[index] -> sec++;
    }
}

void update_min16(uint16_t index){
    update_sec16(index);
    if ((timers16[index] -> sec) > 59){
        (timers16[index] -> sec) = 0;
        timers16[index] -> min++;
    }
}

void update_hour16(uint16_t index){
    update_min16(index);
    if ((timers16[index] -> min) > 59){
        (timers16[index] -> min) = 0;
        timers16[index] -> hour++;
    }
}

void update_day16(uint16_t index){
    update_hour16(index);
    if ((timers16[index] -> hour) > 23){
        (timers16[index] -> hour) = 0;
        timers16[index] -> day++;
    }
}

void update_timers16(void){
    uint16_t i;
    for (i = 0; i < MAX_16_TIMERS; i++){
        if ((timers16[i] -> timer_scale) != 0){
            timers16[i] -> msec++;
            switch (timers16[i] -> timer_scale){
                case MSEC:
                    break;
                case SEC:
                    update_sec16(i);
                    break;
                case MIN:
                    update_min16(i);
                    break;
                case HOUR:
                    update_hour16(i);
                    break;
                case DAY:
                    update_day16(i);
                    break;
            }
        }
    }
}


/* 32 bit timer utils */

void update_sec32(uint32_t index){
    if ((timers32[index] -> msec) > 999){
        (timers32[index] -> msec) = 0;
      timers32[index] -> sec++;
    }
}

void update_min32(uint32_t index){
    update_sec32(index);
    if ((timers32[index] -> sec) > 59){
        (timers32[index] -> sec) = 0;
        timers32[index] -> min++;
    }
}

void update_hour32(uint32_t index){
    update_min32(index);
    if ((timers32[index] -> min) > 59){
        (timers32[index] -> min) = 0;
        timers32[index] -> hour++;
    }
}

void update_day32(uint32_t index){
    update_hour32(index);
    if ((timers32[index] -> hour) > 23){
        (timers32[index] -> hour) = 0;
        timers32[index] -> day++;
    }
}

void update_timers32(void){
    uint32_t i;
    for (i = 0; i < MAX_32_TIMERS; i++){
        if ((timers32[i] -> timer_scale) != 0){
            timers32[i] -> msec++;
            switch (timers32[i] -> timer_scale){
                case MSEC:
                    break;
                case SEC:
                    update_sec32(i);
                    break;
                case MIN:
                    update_min32(i);
                    break;
                case HOUR:
                    update_hour32(i);
                    break;
                case DAY:
                    update_day32(i);
                    break;
            }
        }
    }
}

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

void sysT_8_init(sysTimer8_t *timer){
    timers8[next_timer8] = timer;
    next_timer8++;
}

void sysT_16_init(sysTimer16_t *timer, sysT_scale scale){
    timers16[next_timer16] = timer;
    next_timer16++;
    timer -> timer_scale = scale;
}

void sysT_32_init(sysTimer32_t *timer, sysT_scale scale){
    timers32[next_timer32] = timer;
    next_timer32++;
    timer -> timer_scale = scale;
}

void sysT_16_reset(sysTimer16_t *timer){
    (timer -> msec) = 0;
    (timer -> sec) = 0;
    (timer -> min) = 0;
    (timer -> hour) = 0;
    (timer -> day) = 0;
}

//counter_int tmr_get_count(char *timer_name){
//    int8_t timer_index = get_timer_index(timer_name);
//    if (timer_index != -1){
//        return counters[timer_index];
//    }else{
//        return -1;
//    }
//}
//
//counter_int tmr_get_msec(char *timer_name){
//    int8_t timer_index = get_timer_index(timer_name);
//    if (timer_index != -1){
//        return msec[timer_index];
//    }else{
//        return -1;
//    }
//}
//
//counter_int tmr_get_sec(char *timer_name){
//    int8_t timer_index = get_timer_index(timer_name);
//    if (timer_index != -1){
//        return sec[timer_index];
//    }else{
//        return -1;
//    }
//}
//
//counter_int tmr_get_min(char *timer_name){
//    int8_t timer_index = get_timer_index(timer_name);
//    if (timer_index != -1){
//        return min[timer_index];
//    }else{
//        return -1;
//    }
//}
//
//counter_int tmr_get_hour(char *timer_name){
//    int8_t timer_index = get_timer_index(timer_name);
//    if (timer_index != -1){
//        return hour[timer_index];
//    }else{
//        return -1;
//    }
//}
//
//counter_int tmr_get_day(char *timer_name){
//    int8_t timer_index = get_timer_index(timer_name);
//    if (timer_index != -1){
//        return day[timer_index];
//    }else{
//        return -1;
//    }
//}
//
//void tmr_clear(char *timer_name){
//    int8_t timer_index = get_timer_index(timer_name);
//    if (timer_index != -1){
//        cli();
//        msec[timer_index] = 0;
//        sec[timer_index] = 0;
//        min[timer_index] = 0;
//        hour[timer_index] = 0;
//        day[timer_index] = 0;
//        sei();
//    }
//}
///*TODO timer reset
// * protect everything with cli(), sei()
// */
//
///*TODO maybe just have a function that the user places in a 1 ms ISR
// * instead of using an entire timer
// */
//


void sysT_timer_service(void){
    update_timers8();
    update_timers16();
    update_timers32();
}

ISR(TIMER0_COMPA_vect){
    //PORTB |= 1<<PB5;
    sysT_timer_service();
    //PORTB &= ~(1<<PB5);
}
