/** @weakgroup hardware
 *  @{
 */
/**
 * @file
 * @brief uart interface functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/stm32/usart.h>
#include "hw/uart.h"
#include "config.h"
#include "libs/strings_local.h"

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
 * @brief send named number in human-readable binary format
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
 * @brief init uart
 *
 * init uart with parameters from config.h
 */
void init_uart(void)
{
    rcc_periph_clock_enable(UART_RCC);

    gpio_set_mode(UART_PORT, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, UART_TX);
    gpio_set_mode(UART_PORT, GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT, UART_RX);

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

///@}
