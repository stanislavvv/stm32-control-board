/** @weakgroup shell
 *  @{
 */
/**
 * @file shell_hw.c
 * @brief lowlevel-related shell commands and functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <stdint.h>
#include "strings_local.h"
#include "shell_process.h"
#include "shell_hw.h"
#include "hw.h"
#include "bool.h"

#ifndef UNITTEST

#include "FreeRTOS.h"
#include "st7789.h"

// for spi debug command
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
    (void)(argv);
    (void)(argc);
    send_string("lcdtest begin\r\n");
    if (!lcd_inited)
    {
        send_string("lcd init... ");
        ST7789_Init();
        lcd_inited = TRUE;
        send_string("done\r\n");
    }
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
    uint32_t cr1 = SPI_CR1(LCD_SPI);
    uint32_t cr2 = SPI_CR2(LCD_SPI);
    uint32_t sr = SPI_SR(LCD_SPI);
    uint32_t dr = SPI_DR(LCD_SPI);
    uint32_t i2scfgr = SPI_I2SCFGR(LCD_SPI);
    send_named_bin("CR1", cr1, 4);
    send_named_bin("CR2", cr2, 4);
    send_named_bin(" SR", sr, 4);
    send_named_bin(" DR", dr, 4);
    send_named_bin("i2s", i2scfgr, 4);
    if (argc > 0)
    {
        if (compare_strings(argv[0], "test"))
        {
            send_string("sending test sequence 0... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                spi_send(LCD_SPI, 0);
            }
            send_string("0xff... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                spi_send(LCD_SPI, 0xff);
            }
            send_string("0x55... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                spi_send(LCD_SPI, 0x55);
            }
            send_string("0xAA... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                spi_send(LCD_SPI, 0xAA);
            }
            send_string("0x0F... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                spi_send(LCD_SPI, 0x0F);
            }
            send_string("0xF0... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                spi_send(LCD_SPI, 0xF0);
            }
        }
    }
    send_string("end\r\n");
}

/**
 * @brief show rtos heap usage
 * @param argv, argc - any may be given, none used
 */
void shell_rtos_heap_cmd(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    HeapStats_t stats;
    char s[20] = "\0";
    shell_out_buffer_add("rtos heap stats:");
    vPortGetHeapStats(&stats);

    itoa_u16((uint16_t)stats.xAvailableHeapSpaceInBytes, s);
    shell_out_buffer_add("Avail: ");
    shell_out_buffer_add(s);
    shell_out_buffer_add("\r\n");

    itoa_u16((uint16_t)stats.xSizeOfLargestFreeBlockInBytes, s);
    shell_out_buffer_add("Largest free block bytes: ");
    shell_out_buffer_add(s);
    shell_out_buffer_add("\r\n");

    itoa_u16((uint16_t)stats.xMinimumEverFreeBytesRemaining, s);
    shell_out_buffer_add("Min free bytes: ");
    shell_out_buffer_add(s);
    shell_out_buffer_add("\r\n");
}


#endif

/** @}*/
