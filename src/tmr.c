/**
 * AVR System Timer
 * @file    tmr.c
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 20:47:09
 * @edited  2015-04-21 01:04:13
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

#if defined (__SYS_COUNTER__)
#define NUM_COUNTERS    NUM_USER_COUNTERS + 1
#else
#define NUM_COUNTERS    NUM_USER_COUNTERS
#endif

char names[NUM_COUNTERS][COUNTER_NAME_MAX_LENGTH + 1];
EResolution resolutions[NUM_COUNTERS];
bool in_use[NUM_COUNTERS];

counter_int *counters;
counter_int msec[NUM_COUNTERS];
counter_int sec[NUM_COUNTERS];
counter_int min[NUM_COUNTERS];
counter_int hour[NUM_COUNTERS];
counter_int day[NUM_COUNTERS];

//next available timer
uint8_t next_timer;

/******************************************************************************/
/* Local Utilities                                                            */
/******************************************************************************/

void update_sec(uint8_t index){
    if (msec[index] == 1000){
        msec[index] = 0;
        sec[index]++;
    }

}

void update_min(uint8_t index){
    if (sec[index] == 60){
        sec[index] = 0;
        min[index]++;
    }
}

void update_hour(uint8_t index){
    if (min[index] == 60){
        min[index] = 0;
        hour[index]++;
    }
}

void update_day(uint8_t index){
    if (hour[index] == 24){
        hour[index] = 0;
        day[index]++;
    }
}

int8_t get_timer_index(char *timer_name){
    int8_t i = -1;
    for (i = 0; i < NUM_COUNTERS; i++){
        if (strcmp(names[i],timer_name) == 0){
            break;
        }
    }
    return i;
}

/******************************************************************************/
/* Library Functions                                                          */
/******************************************************************************/
void tmr_init(void){
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
    //insert timer regs for 1284p_available
#endif
#if defined (__SYS_COUNTER__)
    tmr_new_counter("sys_counter",ALL);
#endif
}

int tmr_new_counter(char *timer_name, EResolution resolution){
    //TODO prevent 2 timers with same name
    //TODO return -1 if name already used, or no timer remaining, or name too long
    strcpy(names[next_timer], timer_name);
    resolutions[next_timer] = resolution;
    in_use[next_timer] = true;
    switch (resolution){
        case MSEC:
            counters[next_timer] = msec[next_timer];
            break;
        case SEC:
            counters[next_timer] = sec[next_timer];
            break;
        case MIN:
            counters[next_timer] = min[next_timer];
            break;
        case HOUR:
            counters[next_timer] = hour[next_timer];
            break;
        case DAY:
            counters[next_timer] = day[next_timer];
            break;
        case ALL:
            //TODO
            //nothing for now
            break;
    }
    next_timer++;
    return 1;
}

counter_int tmr_get_count(char *timer_name){
    int8_t timer_index = get_timer_index(timer_name);
    if (timer_index != -1){
        return counters[timer_index];
    }else{
        return -1;
    }
}

counter_int tmr_get_msec(char *timer_name){
    int8_t timer_index = get_timer_index(timer_name);
    if (timer_index != -1){
        return msec[timer_index];
    }else{
        return -1;
    }
}

counter_int tmr_get_sec(char *timer_name){
    int8_t timer_index = get_timer_index(timer_name);
    if (timer_index != -1){
        return sec[timer_index];
    }else{
        return -1;
    }
}

counter_int tmr_get_min(char *timer_name){
    int8_t timer_index = get_timer_index(timer_name);
    if (timer_index != -1){
        return min[timer_index];
    }else{
        return -1;
    }
}

counter_int tmr_get_hour(char *timer_name){
    int8_t timer_index = get_timer_index(timer_name);
    if (timer_index != -1){
        return hour[timer_index];
    }else{
        return -1;
    }
}

counter_int tmr_get_day(char *timer_name){
    int8_t timer_index = get_timer_index(timer_name);
    if (timer_index != -1){
        return day[timer_index];
    }else{
        return -1;
    }
}

void tmr_clear(char *timer_name){
    int8_t timer_index = get_timer_index(timer_name);
    if (timer_index != -1){
        cli();
        msec[timer_index] = 0;
        sec[timer_index] = 0;
        min[timer_index] = 0;
        hour[timer_index] = 0;
        day[timer_index] = 0;
        sei();
    }
}
/*TODO timer reset
 * protect everything with cli(), sei()
 */

/*TODO maybe just have a function that the user places in a 1 ms ISR
 * instead of using an entire timer
 */

ISR(TIMER0_COMPA_vect){
    unsigned char i;
    for (i = 0; i < NUM_COUNTERS; i++){
        if (in_use[i] == true){
            //increment base timer
            msec[i]++;
            switch (resolutions[i]){
                case MSEC:
                    //nothing
                    break;
                case SEC:
                    update_sec(i);
                    break;
                case MIN:
                    update_sec(i);
                    update_min(i);
                    break;
                case HOUR:
                    update_sec(i);
                    update_min(i);
                    update_hour(i);
                    break;
                case DAY:
                case ALL:
                    update_sec(i);
                    update_min(i);
                    update_hour(i);
                    update_day(i);
                    break;
            }
        }
    }
}
