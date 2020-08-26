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
 *
 * name: shell_led_on()
 * @brief switch on led on PC13
 * @param none - any may be given, none used
 * @return none
 *
 */
void shell_led_on(char* argv[], uint16_t argc);

/**
 *
 * name: shell_led_off()
 * @brief switch off led on PC13
 * @param none - any may be given, none used
 * @return none
 *
 */
void shell_led_off(char* argv[], uint16_t argc);

/**
 *
 * name: shell_led_state()
 * @brief say to shell buffer state of led on PC13
 * @param none - any may be given, none used
 * @return none
 *
 */
void shell_led_state(char* argv[], uint16_t argc);

/**
 *
 * name: shell_led
 * @brief control led on PC13 and say to shell buffer its state
 * @param string (on|1|off|0) - on/off led, may be omitted
 * @return none
 *
 */
void shell_led(char* argv[], uint16_t argc);


#endif
