/* copyright https://github.com/stanislavvv/stm32-control-board */
#include "FreeRTOS.h"
#include "task.h"

/* shell processing rtos task */
void task_process_shell(void *args __attribute((unused)));

/*
 *
 * name: shell_send_result
 *
 * send content of shell_output_buffer to uart console and clean
 */
void shell_send_result(void);
