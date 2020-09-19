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

#ifndef HW_H_
#define HW_H_

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
#include "bool.h"

/// switch led on
#define LED_on() gpio_clear(LED_PORT, LED_PIN)

/// switch led off
#define LED_off() gpio_set(LED_PORT, LED_PIN)

/// get led state
#define LED_state() (GPIO_ODR(LED_PORT) && LED_PIN)

#if DEBUG_PRINT==1
    /// debug print
    #define DBG(s) send_string(s)
#else
    #define DBG(s)
#endif // DEBUG

/**
 * @brief return true if uart has received char in register
 * @return bool char received state
 */
#define char_is_recv() (USART_SR(UART) & USART_SR_RXNE) != 0

/**
 * @brief receive char from uart
 * @return received char
 */
#define recv_char() (char)(0xff & usart_recv_blocking(UART))

/**
 * @brief send char to uart
 * @param c - char for sending to uart
 * @return none
 */
static inline void send_char(char c)
{
    usart_send_blocking(UART, (uint16_t)(c));
}

/**
 * @brief send null-terminated string to uart
 * @param s[] - string for sending to uart
 * @return none
 */
void send_string(const char s[]);

/**
 * @brief send named number in human-readable binary
 * @param name - name (max char[10])
 * @param data - sending number up to uint32_t
 * @param nibbles - size of data in nibbles, 1..8
 *
 * useful for debug via uart
 */
void send_named_bin(char name[], uint32_t data, uint8_t nibbles);

/**
 * @brief init hardware related stuff
 */
void init_hw(void);

/**
 * @brief delay to given time in ms
 * @param ms  time in milliseconds up to 65535
 */
static inline void delay_ms(uint16_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief delay for critical section
 * @param x3 delay value
 */
static inline void delay_x3(uint16_t x3)
{
    for (uint16_t i = 0; i < x3; i++)    /* Wait a bit. */
        { __asm__("nop"); }
}

#endif // ifdef HW_H_
/// @}
