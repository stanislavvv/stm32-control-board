/* copyright https://github.com/stanislavvv/stm32-control-board */
#include "FreeRTOS.h"
#include "task.h"
#include "hw.h"
#include "shell_process.h"
#include "shell.h"

/*
 *
 * name: shell_send_result
 *
 * send content of shell_output_buffer to uart console and clean
 */
void shell_send_result(void)
{
    /* char to char sending */
    for (uint16_t i = 0; i < shell_in_lastchar; i++)
    {
        send_char(shell_output_buffer[i]);
    }
    /* cleaning */
    shell_in_lastchar = 0;
    shell_output_buffer[0] = 0;
}

/* shell processing rtos task */
void task_process_shell(void *args __attribute((unused)))
{
    for(;;)
    {
        if (char_is_recv())
        {
            char c = recv_char();
                if (c != 0xa && shell_buffer_add(c)) {
                taskYIELD();
            }
            else
            {
                // at end of line or buffer overflow - process string
                // and send result.
                shell_process(shell_output_buffer, shell_output_buffer);
                shell_send_result();
            }
        } else
        {
            taskYIELD();
        }
    }
}
