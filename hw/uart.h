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
/**
 * @defgroup serial Serial communications
 * Serial port-related functions
 */
/**
 * @ingroup serial
 * @{
 */

#ifndef HW_UART_H_
#define HW_UART_H_

#include <libopencm3/stm32/usart.h>
#include "config.h"
#if (DEBUG == 1)
    #include "hw.h"
#endif

/**
 * @brief return true if uart has received char in register
 * @return bool char received state
 */
#if (DEBUG == 1)
    static inline char char_is_recv(void)
    {
        LED_on();
        return (USART_SR(UART) & USART_SR_RXNE) != 0;
    }
#else
    #define char_is_recv() (USART_SR(UART) & USART_SR_RXNE) != 0
#endif // DEBUG

/**
 * @brief receive char from uart
 * @return received char
 */
#if (DEBUG == 1)
    static inline char recv_char(void)
    {
        LED_off();
        return (char)(0xff & usart_recv_blocking(UART));
    }
#else
    #define recv_char() (char)(0xff & usart_recv_blocking(UART))
#endif // DEBUG

/**
 * @brief send char to uart
 * @param c - char for sending to uart
 * @return none
 */
#define send_char(c) usart_send_blocking(UART, (uint16_t)(c))

/*
 * @brief send null-terminated string to uart
 * @param s[] - string for sending to uart
 * @return none
 */
void send_string(const char s[]);

/**
 * @brief send named number in human-readable binary format
 * @param name - name (max char[10])
 * @param data - sending number up to uint32_t
 * @param nibbles - size of data in nibbles, 1..8
 *
 * useful for debug via uart
 */
void send_named_bin(char name[], uint32_t data, uint8_t nibbles);

/**
 * @brief init uart
 *
 * init uart with parameters from config.h
 */
void init_uart(void);

#endif
///@}
///@}
