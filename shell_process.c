/* copyright https://github.com/stanislavvv/stm32-control-board */

#include <stdint.h>
#include <stdio.h>
#include "bool.h"
#include "strings_local.h"
#include "shell_process.h"

#ifndef UNITTEST

#include "shell_hw.h"

#endif

/* shell input buffer */
char shell_input_buffer[SHELL_MAX_CLI_LENGTH] = "\0";

/* shell input buffer length */
uint16_t shell_in_lastchar = 0;

/* shell output buffer */
char shell_output_buffer[SHELL_MAX_OUT_LENGTH] = "\0";

/* shell output buffer length */
uint16_t shell_out_lastchar = 0;

/* internal functions forward defs */
void shell_get_cmd(char command_s[]);
void shell_out_buffer_add(char s[]);
void shell_hello_cmd(void);

typedef void (*shell_cmd_handler_t)(void);

typedef struct // command + function
{
    const char* cmd_str;
    shell_cmd_handler_t cmd;
} shell_cmd_def_t;

/* shell commands list */
static shell_cmd_def_t cmds[] =
{
    {"hello", shell_hello_cmd},
#ifndef UNITTEST
// not include hardware functions in unit test
    {"led_on", shell_led_on},
    {"led_off", shell_led_off},
#endif
    {NULL, NULL}
};

/* add string to output buffer */
void shell_out_buffer_add(char s[])
{
    uint16_t i = 0;
    while (shell_out_lastchar < SHELL_MAX_OUT_LENGTH && s[i] != 0)
    {
        shell_output_buffer[shell_out_lastchar] = s[i];
        i++;
        shell_out_lastchar++;
    }
}

/* hello command */
void shell_hello_cmd(void)
{
    shell_out_buffer_add("Hello world!!!");
}

/*
 *
 * name: shell_cleanup_output
 *
 * clean shell output buffer
 */
void shell_cleanup_output(void)
{
    shell_out_lastchar = 0;
    for (uint16_t i = 0; i < SHELL_MAX_OUT_LENGTH; i++)
    {
        shell_output_buffer[i] = 0;
    }
}

/* get first word from shell_input_buffer and place it to command_s */
void shell_get_cmd(char command_s[])
{
    uint16_t i = 0;
    while (i < SHELL_MAX_CLI_LENGTH && (char)shell_input_buffer[i] != ' ')
    {
        command_s[i] = shell_input_buffer[i];
        i++;
    }
    command_s[i] = 0;
}

/*
 *
 * name: shell_process
 * @param  outbuffer  output buffer zero-terminated string
 * @param  command_line  command from user input, zero-terminated string
 *
 */
void shell_process(void)
{
    char cmd[SHELL_MAX_CLI_LENGTH];
    shell_cleanup_output();
    shell_get_cmd(cmd);
    uint16_t known_cmd = FALSE;
    uint16_t i = 0;
    while (cmds[i].cmd != NULL)
    {
        if (compare_strings(cmds[i].cmd_str, cmd))
        {
            known_cmd = TRUE;
            cmds[i].cmd();
        }
        i++;
    }
    if (!known_cmd)
    {
        shell_out_buffer_add("UNKNOWN: ");
        shell_out_buffer_add(cmd);
    }
}

/*
 *
 * name: shell_buffer_add
 * @param char c - received character
 * @return bool - non-true on overflow
 *
 */
uint16_t shell_in_buffer_add(char c)
{
    if (shell_in_lastchar >= SHELL_MAX_CLI_LENGTH)
    {
        return 1 == 0;
    }
    else
    {
        shell_input_buffer[shell_in_lastchar] = c;
        shell_in_lastchar++;
        shell_input_buffer[shell_in_lastchar] = '\0';
        return TRUE;
    }
}
