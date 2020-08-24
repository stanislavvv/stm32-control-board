/* copyright https://github.com/stanislavvv/stm32-control-board */
/* shell hardware-related functions */

#ifndef SHELL_HW_H_

#define SHELL_HW_H_

#include <stdint.h>

/*
 *
 * name: shell_led_on
 *
 * switch on led on PC13
 */
void shell_led_on(char* argv[], uint16_t argc);

/*
 *
 * name: shell_led_off
 *
 * switch off led on PC13
 */
void shell_led_off(char* argv[], uint16_t argc);

/*
 *
 * name: shell_led_state()
 *
 * print to shell buffer state of led on PC13
 */
void shell_led_state(char* argv[], uint16_t argc);

/*
 *
 * name: shell_led
 * @param (on|1|off|0) - on/off led
 *
 */
void shell_led(char* argv[], uint16_t argc);


#endif
