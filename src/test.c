/**
 * Testing timer library
 * @file    test.c
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 18:14:58
 * @edited  2015-04-28 01:05:10
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr-uart/uart.h"
#include "tmr.h"

#define F_CPU 16000000L
#include <util/delay.h>
#include <stdio.h>


int main(void){
    uart0_init(UART_BAUD_SELECT(19200,16000000L));
    char buffer[80];

    sysT_init();

    sysTimer16_t print_timer;
    sysT_16_init(&print_timer);

    sei();
    while(1){
        sprintf(buffer, "msec: %u\n", sysT_16_get_ticks(&print_timer));
        uart0_puts(buffer);

        if (sysT_16_get_ticks(&print_timer) > 10000){
            sysT_16_reset(&print_timer);
        }
    }
    return 0;
}
