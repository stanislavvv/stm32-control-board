/** @weakgroup shell
 *  @{
 */
/**
 * @file shell_hw.h
 * @brief hardware-related shell commands and functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#ifndef SHELL_HW_H_

#define SHELL_HW_H_

#include <stdint.h>

/**
 * @brief switch on led on PC13
 * @param argv, argc - any may be given, none used
 */
void shell_led_on(char* argv[], uint16_t argc);

/**
 * @brief switch off led on PC13
 * @param argv, argc - any may be given, none used
 */
void shell_led_off(char* argv[], uint16_t argc);

/**
 * @brief say to shell buffer state of led on PC13
 * @param argv, argc - any may be given, none used
 */
void shell_led_state(char* argv[], uint16_t argc);

/**
 * @brief control led on PC13 and say to shell buffer its state
 * @param argv, argc - single parameter string
 *
 * parameter will processed with content: (on|off|1|0)
 * - on or 1 -- led will be on
 * - off or 0 -- led will be off
 */
void shell_led(char* argv[], uint16_t argc);

#ifndef UNITTEST
// Can't be tested without uC

/**
 * @brief start lcd test
 * @param argv, argc - any may be given, none used
 */
void shell_lcd_test(char* argv[], uint16_t argc);
#endif

#endif

/** @}*/
