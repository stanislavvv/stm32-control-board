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
 * @brief return true if spi can tx new data
 * @param spi for example SPI1
 * @return TRUE if ready for tx
 */
boolean spi_tx_ready(uint32_t spi);

/**
 * @brief send buffer to spi with timeout
 * @param spi  spi port, ex. SPI1 in libopencm3
 * @param buffer  buffer of **bytes** for sending
 * @param length  length of buffer
 * @param timeout  timeout in ticks, portMAX_DELAY and 0 - switch off
 * @return errno - TRUE ok, FALSE - error (bad parameters or timeout)
 *
 * Transmit buffer with given length to spi in two-wire 8-bit mode with timeout
 */
boolean spi_send_buffer_2wire_8bit(uint32_t spi, uint8_t *buffer,
                            uint16_t length, TickType_t timeout);

/**
 * @brief set gpio and other hardware modes
 */
void init_hw(void);

/**
 * @brief show spi registers and may test spi transfer
 * @param argv, argc 'test' will be test spi transfer
 */
void shell_spi_cmd(char* argv[], uint16_t argc);

/**
 * @brief delay to given time in ms
 * @param ms  time in milliseconds up to 65535
 * @return none
 */
static inline void delay_ms(uint16_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

#endif // ifdef HW_H_
/// @}
