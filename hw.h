/** @weakgroup hardware
 *  @{
 */
/**
 * @file hw.h
 * @brief hardware interface functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */


#ifndef UNITTEST
// NORMAL WORK

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "FreeRTOS.h"
#include "task.h"
#include "config_hw.h"


/**
 * switch on led
 */

#define LED_on() gpio_clear(LED_PORT, LED_PIN)
/**
 * switch off led
 */

#define LED_off() gpio_set(LED_PORT, LED_PIN)
/**
 * get led state
 */
#define LED_state() (GPIO_ODR(LED_PORT) && LED_PIN)

/**
 * @brief receive char from uart
 * @return received char
 */
#define recv_char() (char)(0xff & usart_recv_blocking(UART))
/*
static inline char recv_char(void)
{
    return (char) (0xff & usart_recv_blocking(UART));
}
*/

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
 * @brief return true if uart has received char in register
 * @return bool char received state
 */
#define char_is_recv() (USART_SR(UART) & USART_SR_RXNE) != 0


/**
 * @brief delay to given time in ms
 * @param ms  time in milliseconds up to 65535
 * @return none
 */
static inline void delay_ms(uint16_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief delay to given time in rtos ticks
 * @param ticks  time in ticks up to portMAX_DELAY
 * @return none
 */
static inline void delay_ticks(TickType_t ticks)
{
    vTaskDelay(ticks);
}

/**
 * @brief send buffer to spi with timeout
 * @param spi  spi port, ex. SPI1 in libopencm3
 * @param buffer  buffer of bytes for sending
 * @param length  length of buffer
 * @param timeout  timeout in ticks, portMAX_DELAY and 0 - switch off
 * @return errno - 0 (ok) or EBUSY (busy), ETIME (timeout), EIO (error)
 *
 * Transmit buffer with given length to spi in two-wire 8-bit mode with timeout
 */
uint16_t spi_send_buffer_2wire_8bit(uint32_t spi, uint8_t *buffer,
                                    uint16_t length, TickType_t timeout);

/**
 * @brief return true if uart has received char in register
 * @return bool char received state
 */
void init_gpio(void);

#else
// UNIT TESTS
#include <stdint.h>

//ToDo: make mocks for this functions
// dummy realisation for tests.c
char recv_char(void);
void send_char(char c);
void send_string(const char s[]);
uint16_t char_is_recv(void);
void init_gpio(void);

char recv_char(void)
{
    return ' ';
}

void send_char(char c)
{
}

void send_string(const char s[])
{
}

uint16_t char_is_recv(void)
{
    return (1==0);
}

void init_gpio(void)
{
}

//ToDo: make test mocks for this defs
#define LED_on()
#define LED_off()
#define LED_state() 1

#endif

/** @}*/
