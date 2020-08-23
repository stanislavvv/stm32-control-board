/* copyright  */

#ifndef UNITTEST
// NORMAL WORK
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "FreeRTOS.h"
#include "task.h"
#include "config_hw.h"

#define LED_on() gpio_clear(LED_PORT, LED_PIN);
#define LED_off() gpio_set(LED_PORT, LED_PIN);

/*
 *
 * name: recv_chars
 * @return received char
 *
 * receive char from uart
 */
char recv_char(void);

/*
 *
 * name: send_char
 * @param char c - char for sending to uart
 *
 * send char to uart
 */
void send_char(char c);

/*
 *
 * name: send_string
 * @param char s[] - string for sending to uart
 *
 * send null-terminated string to uart
 */
void send_string(const char s[]);

/*
 *
 * name: char_is_recv
 * @return bool char received state
 *
 * return true if uart has received char in register
 */
uint16_t char_is_recv(void);

/*
 *
 * name: init_gpio
 *
 * set gpio and other hardware modes
 */
void init_gpio(void);

#else
// UNIT TESTS

// dummy realisation for tests.c
char recv_char(void);
void send_char(char c);
void send_string(char s[]);
uint16_t char_is_recv(void);
void init_gpio(void);

char recv_char(void) {
    return " ";
}

void send_char(char c)
{
}

void send_string(const char s[])
{
}

uint16_t char_is_recv(void)
{
    return (1==0)
}

void init_gpio(void)
{
}

#define LED_on()
#define LED_off()


#endif
