/** @weakgroup hardware
 *  @{
 */
/**
 * @file
 * @brief hardware interface functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "bool.h"
#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "strings_local.h"
#include "shell.h"
#include "hw.h"
#include "hw/spi.h"

/**
 * @brief send null-terminated string to uart
 * @param s[] - string for sending to uart
 */
void send_string(const char s[])
{
    uint16_t i = 0;
    while (s[i] != 0)
    {
        send_char(s[i]);
        i++;
    }
}

/**
 * @brief send named number in human-readable binary
 * @param name - name (max char[10])
 * @param data - sending number up to uint32_t
 * @param nibbles - size of data in nibbles, 1..8
 *
 * useful for debug via uart
 */
void send_named_bin(char name[], uint32_t data, uint8_t nibbles)
{
    char sendbuffer[50];
    uint8_t c = 0;
    uint8_t n = (uint8_t)strlen_local(name);
    if (n > 10) { n = 10; };
    for (uint8_t i = 0; i < n; i++)
    {
        sendbuffer[c] = name[i];
        c++;
    }
    sendbuffer[c] = ':'; c++;
    sendbuffer[c] = ' '; c++;
    i2bin(data, &sendbuffer[c], nibbles);
    c = (uint8_t)strlen_local(sendbuffer);
    sendbuffer[c] = '\r'; c++;
    sendbuffer[c] = '\n'; c++;
    sendbuffer[c] = 0; c++;
    send_string(sendbuffer);
}

/**
 * @brief init all clock
 */
static inline void init_rcc(void)
{
    /* switch to quartz 8MHz + pll 72MHz */
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"

    /* periferial clock */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(UART_RCC);
}

/**
 * @brief set generic gpio pin modes
 */
static inline void init_gpio(void)
{
        /* LED on PC13 */
    gpio_set_mode(
        LED_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        LED_PIN);
    /* led is on after default 0 on pin reset - switch off */
    LED_off();

    // encoder init for software polling
    /* encoder button on PA15 */
    gpio_set_mode(
        ENCODER_BUTTON_PORT,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_PULL_UPDOWN,
        ENCODER_BUTTON_PIN);
    /* pull up button */
    uint32_t port = GPIO_ODR(GPIOA);
    GPIO_BSRR(GPIOA) = (port & GPIO15);

    /* encoder quadrature on PB4,PB5 */
    gpio_set_mode(
        ENCODER_QUAD_PORT,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT,
        ENCODER_QUAD_PIN1|ENCODER_QUAD_PIN2);
}

/**
 * @brief init uart
 *
 * init uart with parameters from config.h
 */
static inline void init_uart(void)
{
        /* uart tx PA9 */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
    /* uart rx PA10 */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

    /* setup uart parameters */
    usart_set_baudrate(UART, UART_SPEED);
    usart_set_databits(UART, 8);
    usart_set_stopbits(UART, USART_STOPBITS_1);
    usart_set_parity(UART, USART_PARITY_NONE);
    usart_set_flow_control(UART, USART_FLOWCONTROL_NONE);
    usart_set_mode(UART, USART_MODE_TX_RX);

    /* enable uart */
    usart_enable(UART);
}

/**
 * @brief init hardware related stuff
 */
void init_hw(void)
{
    init_rcc();
    init_gpio();
    init_uart();
#if BOOT_VERBOSE==1
    send_string("uart initalized\r\n");
#endif

    init_spi();
#if BOOT_VERBOSE==1
    send_string("spi enabled:\r\n");
    send_named_bin("CR1 actual", SPI_CR1(LCD_SPI), 4);
#endif

#if BOOT_VERBOSE==1
    send_string("hw init end\r\n");
#endif
}

/**
 * @brief show spi registers and may test spi transfer
 * @param argv, argc 'test' will be test spi transfer
 */
void shell_spi_cmd(char* argv[], uint16_t argc)
{
    if (argc > 0)
    {
        if (compare_strings(argv[0], "test"))
        {
            /*
            we send string directly to uart
            for indication of current value
            */
            send_string("sending test sequence 0... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                SPI_SEND(LCD_SPI, 0);
            }
            send_string("0xff... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                SPI_SEND(LCD_SPI, 0xff);
            }
            send_string("0x55... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                SPI_SEND(LCD_SPI, 0x55);
            }
            send_string("0xAA... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                SPI_SEND(LCD_SPI, 0xAA);
            }
            send_string("0x0F... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                SPI_SEND(LCD_SPI, 0x0F);
            }
            send_string("0xF0... ");
            for (uint16_t i = 0; i<=65534; i++)
            {
                SPI_SEND(LCD_SPI, 0xF0);
            }
        }
    }
    else
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
    }
    shell_out_buffer_add("end\r\n");
}

/// @}
