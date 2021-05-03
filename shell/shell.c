/** @weakgroup shell
 *  @{
 */
/**
 * @file
 * @brief shell functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 * command list see in #cmds[]
 */

#include <stdint.h>
#include "libs/strings_local.h"
#include "shell.h"
#include "shell_cmds.h"

#ifndef UNITTEST
    #include "FreeRTOS.h"
    #include "task.h"
    #include "hw.h"
    #include "hw/spi.h"
#else
    #include <stddef.h>
#endif

/// shell input buffer, received from uart
char shell_input_buffer[SHELL_MAX_CLI_LENGTH] = "\0";

/// shell cli current length
uint16_t shell_in_lastchar = 0;

/// shell output buffer, will be sent to uart
char shell_output_buffer[SHELL_MAX_OUT_LENGTH] = "\0";

/// shell output buffer current length
uint16_t shell_out_lastchar = 0;

/**
 * @brief get command name
 * @param command_s[] - command name will be place here
 *
 * get first word from shell_input_buffer and place it to command_s */
static inline void shell_get_cmd(char command_s[])
{
    uint16_t i = 0;
    while (i < SHELL_MAX_CLI_LENGTH && (char)shell_input_buffer[i] != ' ')
    {
        command_s[i] = shell_input_buffer[i];
        i++;
    }
    command_s[i] = 0;
}

/**
 * @brief add string to output buffer
 * @param s[] string which content will be added to {@link #shell_output_buffer}
 */
void shell_out_buffer_add(const char s[])
{
    uint16_t i = 0;
    while (shell_out_lastchar < SHELL_MAX_OUT_LENGTH && s[i] != 0)
    {
        shell_output_buffer[shell_out_lastchar] = s[i];
        i++;
        shell_out_lastchar++;
    }
}

/**
 * @brief add char to {@link #shell_input_buffer}
 * @param c - received character
 * @return boolean - non-true on overflow
 */
boolean shell_in_buffer_add(char c)
{
    if (shell_in_lastchar >= SHELL_MAX_CLI_LENGTH)
    {
        return FALSE;
    }
    else
    {
        shell_input_buffer[shell_in_lastchar] = c;
        shell_in_lastchar++;
        shell_input_buffer[shell_in_lastchar] = '\0';
        return TRUE;
    }
}

/**
 * @brief clean shell output buffer
 *
 * clean {@link #shell_output_buffer} for later use
 */
void shell_cleanup_output(void)
{
    shell_out_lastchar = 0;
    for (uint16_t i = 0; i < SHELL_MAX_OUT_LENGTH; i++)
    {
        shell_output_buffer[i] = 0;
    }
}

/**
 * @brief shell cli processing
 * see in {@link #shell_input_buffer} and run corresponding commands
 * from {@link #cmds} with parameters
 */
void shell_process(void)
{
    char cmd[SHELL_MAX_CLI_LENGTH];
    char *cmd_argv[SHELL_MAX_ARGS];
    uint16_t cmd_argc = 0;

    shell_cleanup_output();
    shell_get_cmd(cmd);

    /* temporary vars */
    uint16_t known_cmd = FALSE;
    uint16_t i = 0;
    char c;

    while (cmds[i].cmd != NULL)
    {
        if (compare_strings(cmds[i].cmd_str, cmd))
        {
            known_cmd = TRUE;
            uint16_t cmdpos = strlen_local(cmd);
            do
            {
                c = shell_input_buffer[cmdpos++];
                if ((c == '\0'))
                {
                    break;
                }
                if ((c == ' '))
                {
                    cmd_argv[cmd_argc++] = shell_input_buffer+cmdpos;
                }
            } while (c != '\0' && (cmd_argc < SHELL_MAX_ARGS));

            /* arguments must be separate by zero to be parsable like strings */
            for (uint16_t ch = 0; ch < SHELL_MAX_CLI_LENGTH; ch++)
            {
                if (shell_input_buffer[ch] == ' ')
                {
                    shell_input_buffer[ch] = '\0';
                }
            }
            cmds[i].cmd(cmd_argv, cmd_argc);
        }
        i++;
    }
    if (!known_cmd)
    {
        shell_out_buffer_add("UNKNOWN: ");
        shell_out_buffer_add(cmd);
        shell_out_buffer_add("\r\n");
    }
}

#ifndef UNITTEST

/**
 * @brief send content of {@link #shell_output_buffer} to uart
 * will send to uart {@link #shell_output_buffer} and clean
 * {@link #shell_input_buffer} and {@link #shell_output_buffer}
 */
void shell_send_result(void)
{
    send_string(shell_output_buffer);
    /* cleaning */
    shell_out_lastchar = 0;
    shell_output_buffer[0] = 0;
    shell_in_lastchar = 0;
    shell_input_buffer[0] = 0;
}

/**
 * @brief shell processing rtos task
 *
 * @ingroup hwrtos_shell
 */
void shell_task(void *args __attribute((unused)))
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

/** @todo potential uart buffer overrun here, need to refactor this condition
 * to not call taskYIELD() before end of line
 */
            if (
                !(
                    c != 0xa &&
                    c != 0xd &&
                    shell_in_buffer_add(c)
                  )
                )
/*            {
                taskYIELD();
            }
            else*/
            {
                // at end of input line or buffer overflow - process string
                // and send result.
                shell_process();

                #if SHELL_ECHO==1
                send_string("\r\n"); // shift output down
                #endif

                shell_send_result();
            }
        }
        else
        {
            taskYIELD();
        }
    }
}
#endif // ifndef UNITTEST

///@}
