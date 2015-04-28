/**
 * Testing timer library
 * @file    test.c
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 18:14:58
 * @edited  2015-04-27 23:53:27
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr-uart/uart.h"
#include "tmr.h"

//TODO remove after testing
#define F_CPU 16000000L
#include <util/delay.h>
#include <stdio.h>
//TODO remove after testing


int main(void){
    uart0_init(UART_BAUD_SELECT(19200,16000000L));
    char buffer[80];

    sysT_init();
    //sysTimer16_t mins;
    sysTimer16_t print_timer;

    //sysT_16_init(&mins, MIN);
    sysT_16_init(&print_timer,MIN);

    sei();
    DDRB |= 1<<PB5;
    while(1){
        //if (print_timer.msec > 500){
            //sysT_16_reset(&print_timer);
        //if (print_timer.msec >= 500){
            //sysT_16_reset(&print_timer);

            //sprintf(buffer, "msec: %lu ", print_timer.msec);
            //uart0_puts(buffer);
            //sprintf(buffer, "sec: %lu ", print_timer.sec);
            //uart0_puts(buffer);
            //sprintf(buffer, "min: %lu\n", print_timer.min);
            //uart0_puts(buffer);
            //_delay_ms(500);
        if (print_timer.msec > 500){
            //sysT_16_reset(&print_timer);
        }
            sprintf(buffer, "msec: %d ", sysT_16_get_msec(&print_timer));
            uart0_puts(buffer);
            sprintf(buffer, "sec: %d ", sysT_16_get_sec(&print_timer));
            uart0_puts(buffer);
            sprintf(buffer, "min: %d\n", sysT_16_get_min(&print_timer));
            uart0_puts(buffer);
            //_delay_ms(500);
        //}
        //    //unsigned char test = 0;
        //    //uart0_putc('c');
        //    //uint8_t i = 0;
        //    //while (i++ < 200){
        //}
        //PORTB |= 1<<PB5;
        //_delay_ms(1000);
        //uart0_putc('a');
        //PORTB &= ~(1<<PB5);
        //_delay_ms(1000);


            //}
            //sprintf(buffer, "SEC: %d   ", mins.sec);
            //uart0_puts(buffer);

            //sprintf(buffer, "MIN: %d\n", mins.min);
            //uart0_puts(buffer);

            //sysT_16_reset(&print_timer);
        //}
    }
    return 0;
}
