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
#ifdef SHELL_ECHO
    send_string("\r\n"); // shift output down
#endif
    send_string(shell_output_buffer);
    /* cleaning */
    shell_out_lastchar = 0;
    shell_output_buffer[0] = 0;
    shell_in_lastchar = 0;
    shell_input_buffer[0] = 0;
}

/* shell processing rtos task */
void task_process_shell(void *args __attribute((unused)))
{
    send_string("shell started\r\n");
    for (;;)
    {
        if (char_is_recv())
        {
            char c = recv_char();
#ifdef SHELL_ECHO
            send_char(c);
#endif
            if (c != 0xa && c != 0xd && shell_in_buffer_add(c))
            {
                taskYIELD();
            }
            else
            {
                // at end of line or buffer overflow - process string
                // and send result.
                shell_process();
                shell_send_result();
            }
        }
        else
        {
            taskYIELD();
        }
    }
}
