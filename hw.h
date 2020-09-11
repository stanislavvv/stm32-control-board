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
#include "config.h"

/// switch led on
#define LED_on() gpio_clear(LED_PORT, LED_PIN)

/// switch led off
#define LED_off() gpio_set(LED_PORT, LED_PIN)

/// get led state
#define LED_state() (GPIO_ODR(LED_PORT) && LED_PIN)

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
 * @brief set gpio and other hardware modes
 */
void init_hw(void);


#endif // ifdef HW_H_
/// @}
