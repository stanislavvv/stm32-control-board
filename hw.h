/* copyright https://github.com/stanislavvv/stm32-control-board */

/* hardware interface functions */


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
 *
 * name: recv_chars
 * @brief receive char from uart
 * @param none
 * @return received char
 *
 */
char recv_char(void);

/**
 *
 * name: send_char
 * @brief send char to uart
 * @param char c - char for sending to uart
 * @return none
 *
 */
void send_char(char c);

/**
 *
 * name: send_string
 * @brief send null-terminated string to uart
 * @param char s[] - string for sending to uart
 * @return none
 *
 */
void send_string(const char s[]);

/**
 *
 * name: char_is_recv
 * @return bool char received state
 *
 * return true if uart has received char in register
 */
uint16_t char_is_recv(void);

/**
 *
 * name: char_is_recv
 * @brief return true if uart has received char in register
 * @param none
 * @return bool char received state
 *
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
