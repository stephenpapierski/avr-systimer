/**
 * Testing timer library
 * @file    test.c
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 18:14:58
 * @edited  2015-04-25 01:40:36
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
    //uart0_init(UART_BAUD_SELECT(19200,16000000L));
    //char buffer[80];

    //sysT_init();
    //sysTimer16_t mins;
    //sysTimer16_t print_timer;

    //sysT_16_init(&mins, MIN);
    //sysT_16_init(&print_timer,MSEC);

    //sei();
    DDRB |= 1<<PB5;
    while(1){
        //if (print_timer.msec > 3000){
            //uart0_puts("working\n");
            //unsigned char test = 0;
            //uart0_putc('c');
            //uint8_t i = 0;
            //while (i++ < 200){
        PORTB |= 1<<PB5;
        _delay_ms(1000);
        uart0_putc('a');
        PORTB &= ~(1<<PB5);
        _delay_ms(1000);


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
