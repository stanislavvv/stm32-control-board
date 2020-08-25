/* copyright https://github.com/stanislavvv/stm32-control-board */
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


#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )
/**
 * stuff for freertos - catch stack overflow error and hang
 */
    extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );

    void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
    {
        /* This function will get called if a task overflows its stack. */
        ( void ) pxTask;
        ( void ) pcTaskName;
        for (;;) { };
    }
#endif

/**
 * @brief main procedure
 * init hardware, create FreeRTOS tasks and run scheduler
 */
int main(void)
{

    init_gpio();

    xTaskCreate(task_process_shell, "shell", 300, NULL, 1, NULL);
    vTaskStartScheduler();

    for (;;) { };
    return 0;
}

// End
