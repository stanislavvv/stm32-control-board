/* copyright  */
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
