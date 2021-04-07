/** @weakgroup shell
 *  @{
 */
/**
 * @file
 * @brief shell commands
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 * command list see in #cmds[]
 */

#ifndef SHELL_CMDS_H_
#define SHELL_CMDS_H_

#include <stdint.h>
#include "shell.h"

#ifndef UNITTEST
    #include "lcd/lcd.h"
#else
    #include <stddef.h>
#endif

/**
 * @brief send 'Hello World!!!' string as shell output
 * @param argv, argc -- any strings or none
 */
void shell_hello_cmd(char* argv[], uint16_t argc);

/**
 * @brief send list of available commands
 * @param argv, argc -- any strings or none
 */
void shell_ls_cmd(char* argv[], uint16_t argc);


#ifdef UNITTEST

/**
 * @brief arguments test command
 * @param argv, argc -- any strings or none
 *
 * Send arguments count and its contents.
 *
 * This is reference procedure for argument processing.
 */
void args_cmd( char* argv[], uint16_t argc );

#else // ifdef UNITTEST

/**
 * @brief show rtos heap usage
 * @param argv, argc - any may be given, none used
 *
 * @ingroup hwrtos_shell
 */
void shell_rtos_heap_cmd(char* argv[], uint16_t argc);

/**
 * @brief lcd test shell command
 * @param argv, argc 'test' will be test spi transfer
 */
static inline void shell_lcd_cmd(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    DBG("lcd test...\r\n");
    LCD_TEST();
    DBG("end\r\n");
}

/**
 * @brief blink without interrupts with delay_nop()
 * @param argv, argc -- any strings or none
 *
 * will blink by onboard LED with
 */
void shell_delay_noop(char* argv[], uint16_t argc);

#endif // ifdef UNITTEST


/// shell commands list
static shell_cmd_def_t cmds[] =
{
    {"ls",        shell_ls_cmd},
#ifndef UNITTEST
// hardware and rtos related commands
    {"free",      shell_rtos_heap_cmd},
#ifdef LCD_SPI
    {"spi",       shell_spi_cmd},
//    {"lcdclk",    shell_lcdclk_cmd},
#endif
    {"lcd",       shell_lcd_cmd},
    {"dlyled",    shell_delay_noop},
#else
    {"args",      args_cmd},
#endif
    {"hello",     shell_hello_cmd},
    {NULL, NULL}
};


#endif // SHELL_CMDS_H_

///@}
