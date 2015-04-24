/**
 * Testing timer library
 * @file    test.c
 * @author  Stephen Papierski <stephenpapierski@gmail.com>
 * @date    2015-04-20 18:14:58
 * @edited  2015-04-21 01:38:30
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr-uart/uart.h"
#include "tmr.h"

//TODO remove after testing
#include <stdio.h>
//TODO remove after testing


int main(void){
    uart0_init(UART_BAUD_SELECT(9600,16000000L));
    sei();
    tmr_init();
    tmr_new_counter("print_tmr", MIN);
    tmr_new_counter("hours", HOUR);
    char buffer[80];
    uart0_puts("Starting...");
    while(1){
        if (tmr_get_min("print_tmr") > 4){
            tmr_clear("print_tmr");

            uart0_puts("Time min: ");
            sprintf(buffer,"%d", tmr_get_min("hours"));
            uart0_puts(buffer);

            uart0_puts(" hour: ");
            sprintf(buffer,"%d", tmr_get_hour("hours"));
            uart0_puts(buffer);
            uart0_puts("\n");
        }

    }
    return 0;
}
