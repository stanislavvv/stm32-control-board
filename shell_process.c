/** @weakgroup shell
 *  @{
 */
/**
 * @file shell_process.c
 * @brief shell processing functions and shell commands
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <stdint.h>
#include <stdio.h>
#include "bool.h"
#include "strings_local.h"
#include "shell_process.h"

#ifndef UNITTEST

#include "shell_hw.h"

#endif

/**
 * shell input buffer, received from uart
 */
char shell_input_buffer[SHELL_MAX_CLI_LENGTH] = "\0";

/**
 * shell input buffer length
 */
uint16_t shell_in_lastchar = 0;

/**
 * shell output buffer, will be sent to uart
 */
char shell_output_buffer[SHELL_MAX_OUT_LENGTH] = "\0";

/**
 * shell output buffer length
 */
uint16_t shell_out_lastchar = 0;

/* internal functions forward defs */
void shell_get_cmd(char command_s[]);
void shell_get_args(uint16_t cmdlen);
void shell_hello_cmd(char* argv[], uint16_t argc);
void args_cmd(char* argv[], uint16_t argc);

/**
 * shell command handler type
 */
typedef void (*shell_cmd_handler_t)(char* argv[], uint16_t argc);

/**
 * shell command structure, used in command list
 */
typedef struct // command + function
{
    const char* cmd_str;
    shell_cmd_handler_t cmd;
} shell_cmd_def_t;

/**
 * shell commands list
 */
static shell_cmd_def_t cmds[] =
{
    {"hello",     shell_hello_cmd},
    {"ls",        shell_cmds},
    {"args",      args_cmd},
#ifndef UNITTEST
// not include hardware functions in unit test

/* functionality covered by 'led' command
    {"led_on",    shell_led_on},
    {"led_off",   shell_led_off},
    {"led_state", shell_led_state},
*/
    {"led",       shell_led},
    {"lcdtest",   shell_lcd_test},
    {"spi",       shell_spi_command},
    {"free",      shell_rtos_heap_cmd},
#endif
    {NULL, NULL}
};

/**
 * @brief add string to output buffer
 * @param s[] string which content will be added to {@link #shell_output_buffer}
 * @return none
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
 * @brief send 'Hello World!!!' string as shell output
 * @param argv, argc -- any strings or none
 */
void shell_hello_cmd(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    shell_out_buffer_add("Hello world!!!\r\n");
}

/**
 * @brief send list of available commands
 * @param argv, argc -- any strings or none
 */
void shell_cmds(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    uint16_t i = 0;
    shell_out_buffer_add("\r\n-- commands --\r\n");
    while (cmds[i].cmd != NULL)
    {
        shell_out_buffer_add(cmds[i].cmd_str);
        shell_out_buffer_add("\r\n");
        i++;
    }
}

/**
 * @brief argumets test command
 * @param argv, argc -- any strings or none
 *
 * send arguments count and its contents
 */
void args_cmd( char* argv[], uint16_t argc )
{
    uint16_t i;
    char num[6];
    shell_out_buffer_add("arguments count: ");
    itoa_u16(argc, num);
    shell_out_buffer_add(num);
    shell_out_buffer_add("\r\n");
    for (i = 0; i< argc; ++i)
    {
        shell_out_buffer_add("argument ");
        itoa_u16(i, num);
        shell_out_buffer_add(num);
        shell_out_buffer_add(": ");
        shell_out_buffer_add(argv[i]);
        shell_out_buffer_add("\r\n");
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
 * @brief get command name
 * @param command_s[] - command name will be place here
 * @return none
 * get first word from shell_input_buffer and place it to command_s */
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
            /*Ok that is our function! Let's parse args*/
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

            /*arguments must be separate by zero to be parsable like strings*/
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

/**
 * @brief add char to {@link #shell_input_buffer}
 * @param c - received character
 * @return boolean - non-true on overflow
 */
boolean shell_in_buffer_add(char c)
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

/** @}*/
