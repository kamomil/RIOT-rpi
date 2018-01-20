/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

//#include <stdio.h>

#include "thread.h"
#include "xtimer.h"

char second_thread_stack[THREAD_STACKSIZE_MAIN];

void *second_thread(void *arg)
{

    void* a = arg;
    arg = a;
    volatile i = 0;
    while(1){
        for(i=0;i<1000000;i++)
            ;
        SetGpio(47,0);
        for(i=0;i<1000000;i++)
            ;
        SetGpio(47,1);
        hexstring(0x87654321);
        xtimer_sleep(5);
    }

}


int main(void)
{
	SetGpioFunction(47, 1);			// RDY led

	//for some reason calling this function is necessary for the uart to work
	//initFB();

	SetGpio(47, 1);
	//videotest();


    /*kernel_pid_t pid = */thread_create(second_thread_stack, sizeof(second_thread_stack),
                                     THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                     second_thread, NULL, "pong");

    //    puts("Hello World!");

    uart_init();
    hexstring(0x12345678);

    //printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    //printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    //prvSetupTimerInterrupt();
    int soft;
    int hard;
    int val;
    volatile i = 0;
    while(1){
        for(i=0;i<100000;i++)
            ;
        SetGpio(47,0);
        for(i=0;i<100000;i++)
            ;
        SetGpio(47,1);
        timer_vals(&soft,&hard,&val);
        hexstring(0xabcdefab);
        //hexstring(soft);
        //hexstring(hard);
        //hexstring(val);


        xtimer_sleep(5);
    }
    return 0;
}
