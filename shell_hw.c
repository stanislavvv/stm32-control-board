/**
 * @file shell_hw.c
 * @brief hardware-related shell commands and functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

/* shell hardware-related functions */
#include <stdint.h>
#include "strings_local.h"
#include "shell_process.h"
#include "shell_hw.h"
#include "hw.h"
#include "bool.h"

#ifndef UNITTEST

#include "st7789.h"

/**
 * lcd initialization flag
 */
boolean lcd_inited = FALSE;

#endif

/**
 * @brief switch on led on PC13
 * @param none - any may be given, none used
 * @return none
 */
void shell_led_on(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    LED_on();
}

/**
 * @brief switch off led on PC13
 * @param none - any may be given, none used
 * @return none
 */
void shell_led_off(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    LED_off();
}

/**
 * @brief say to shell buffer state of led on PC13
 * @param none - any may be given, none used
 * @return none
 */
void shell_led_state(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    if (LED_state() == 0)
    {
        shell_out_buffer_add("LED on\r\n");
    }
    else
    {
        shell_out_buffer_add("LED off\r\n");
    }
}

/**
 * @brief control led on PC13 and say to shell buffer its state
 * @param string (on|1|off|0) - on/off led, may be omitted
 * @return none
 */
void shell_led(char* argv[], uint16_t argc)
{
    if (argc > 0)
    {
        if (compare_strings(argv[0], "on") || compare_strings(argv[0], "1"))
        {
            LED_on();
        }
        if (compare_strings(argv[0], "off") || compare_strings(argv[0], "9"))
        {
            LED_off();
        }
    }
    shell_led_state(argv, argc);
}

#ifndef UNITTEST
// Can't be tested without uC

/**
 * @brief start lcd test
 * @param none - any may be given, none used
 * @return none
 */
void shell_lcd_test(char* argv[], uint16_t argc)
{
    if (!lcd_inited)
    {
        ST7789_Init();
        lcd_inited = TRUE;
        send_string("lcd inited...\r\n");
    }
    (void)(argv);
    (void)(argc);
    send_string("lcd testing...\r\n");
    ST7789_Test();
    send_string("lcd end\r\n");
}
#endif
