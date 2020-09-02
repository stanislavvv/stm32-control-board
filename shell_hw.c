/** @weakgroup shell
 *  @{
 */
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

// spi debug command
#include <libopencm3/stm32/spi.h>

/**
 * lcd initialization flag
 */
boolean lcd_inited = FALSE;

#endif

/**
 * @brief switch on led on PC13
 * @param argv, argc - any may be given, none used
 */
void shell_led_on(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    LED_on();
}

/**
 * @brief switch off led on PC13
 * @param argv, argc - any may be given, none used
 */
void shell_led_off(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    LED_off();
}

/**
 * @brief say to shell buffer state of led on PC13
 * @param argv, argc - any may be given, none used
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
 * @param argv, argc - single parameter string
 *
 * parameter will processed with content: (on|off|1|0)
 * - on or 1 -- led will be on
 * - off or 0 -- led will be off
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
 * @param argv, argc - any may be given, none used
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

/**
 * @brief show spi registers and may test spi transfer
 * @param argv, argc 'test' will be test spi transfer
 */
void shell_spi_command(char* argv[], uint16_t argc)
{
    send_string("spi regs:\r\n");
    uint32_t cr1 = SPI_CR1(ST7789_SPI);
    uint32_t cr2 = SPI_CR2(ST7789_SPI);
    uint32_t sr = SPI_SR(ST7789_SPI);
    uint32_t dr = SPI_DR(ST7789_SPI);
    uint32_t i2scfgr = SPI_I2SCFGR(ST7789_SPI);
    send_named_bin("CR1", cr1, 4);
    send_named_bin("CR2", cr2, 4);
    send_named_bin(" SR", sr, 4);
    send_named_bin(" DR", dr, 4);
    send_named_bin("i2s", i2scfgr, 4);
    /* ToDo - send test sequence on parameter 'test'
    if (argc > 0)
    {
        if (compare_strings(argv[0], "test")) {
            send_string("sending test sequence... ");
            for (uint16_t i = 0; )
            {
            }
        }
        send_string("end\r\n");
    }
    */
}

#endif

/** @}*/
