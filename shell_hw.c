/* copyright https://github.com/stanislavvv/stm32-control-board */
/* shell hardware-related functions */
#include "shell_hw.h"
#include "hw.h"

/*
 *
 * name: shell_led_on()
 *
 * switch on led on PC13
 */
void shell_led_on(void)
{
    LED_on();
}

/*
 *
 * name: shell_led_off()
 *
 * switch off led on PC13
 */
void shell_led_off(void)
{
    LED_off();
}
