/** @weakgroup shell
 *  @{
 */
/**
 * @file shell.c
 * @brief rtos shell task
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */
/* copyright https://github.com/stanislavvv/stm32-control-board */
#include "FreeRTOS.h"
#include "task.h"
#include "hw.h"
#include "shell_process.h"
#include "shell.h"

/**
 * @brief send content of {@link #shell_output_buffer} to uart
 * will send to uart {@link #shell_output_buffer} and clean
 * {@link #shell_input_buffer} and {@link #shell_output_buffer}
 */
void shell_send_result(void)
{
#if SHELL_ECHO==1
    send_string("\r\n"); // shift output down
#endif
    send_string(shell_output_buffer);
    /* cleaning */
    shell_out_lastchar = 0;
    shell_output_buffer[0] = 0;
    shell_in_lastchar = 0;
    shell_input_buffer[0] = 0;
}

/**
 * @addtogroup rtos
 * @brief shell processing rtos task
 * @param args - no parameters used
 */
void task_process_shell(void *args __attribute((unused)))
{
    send_string("shell started\r\n");
    for (;;)
    {
        if (char_is_recv())
        {
            char c = recv_char();
#if SHELL_ECHO==1
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

/** @}*/
