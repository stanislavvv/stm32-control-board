/* 
 * The LED on PC13 is sending string s content in Morse code
 */
#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#ifdef STM32F1
#include "init_f103.h"
#else
#include "init_f303.h"
#endif


#include "morse.h"

static char s[] = "Hello world!!!";

#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )
    extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );

    void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
    {
        /* This function will get called if a task overflows its stack. */
        ( void ) pxTask;
        ( void ) pcTaskName;
        for( ;; );
    }
#endif

static void
task_hello(void *args __attribute((unused))) {
    for (;;) {
        send_string(s);
        lspc();
    }
}


int
main(void) {

    init_gpio();

    xTaskCreate(task_hello,"hello",100,NULL,configMAX_PRIORITIES-1,NULL);
    vTaskStartScheduler();

    for (;;);
    return 0;
}

// End
