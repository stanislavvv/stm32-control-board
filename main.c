/* copyright  */
/*
 * Control board experiments
 */
#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "config_hw.h"
#include "hw.h"

#include "shell.h"


/* stuff for freertos - catch stack overflow error and hang */
#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )
    extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );

    void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
    {
        /* This function will get called if a task overflows its stack. */
        ( void ) pxTask;
        ( void ) pcTaskName;
        for (;;) { };
    }
#endif


int main(void)
{

    init_gpio();

    xTaskCreate(task_process_shell, "shell", 200, NULL, 1, NULL);
    vTaskStartScheduler();

    for (;;) { };
    return 0;
}

// End
