/* copyright https://github.com/stanislavvv/stm32-control-board */

/* shell hardware-related functions */
#include <stdint.h>
#include "strings_local.h"
#include "shell_process.h"
#include "shell_hw.h"
#include "hw.h"

/*
 *
 * name: shell_led_on()
 * @brief switch on led on PC13
 * @param none - any may be given, none used
 * @return none
 *
 */
void shell_led_on(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    LED_on();
}

/*
 *
 * name: shell_led_off()
 * @brief switch off led on PC13
 * @param none - any may be given, none used
 * @return none
 *
 */
void shell_led_off(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    LED_off();
}

/*
 *
 * name: shell_led_state()
 * @brief say to shell buffer state of led on PC13
 * @param none - any may be given, none used
 * @return none
 *
 */
void shell_led_state(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    if (LED_state() == 0)
    {
        shell_out_buffer_add("LED on\r\n");
    }
    else
    {
        shell_out_buffer_add("LED off\r\n");
    }
}

/*
 *
 * name: shell_led
 * @brief control led on PC13 and say to shell buffer its state
 * @param string (on|1|off|0) - on/off led, may be omitted
 * @return none
 *
 */
void shell_led(char* argv[], uint16_t argc)
{
    if (argc > 0) {
        char a[2] = "x";
        if (compare_strings(argv[0], "on") || compare_strings(argv[0], "1")) {
            LED_on();
            a[0] = '1';
        }
        if (compare_strings(argv[0], "off") || compare_strings(argv[0], "9")) {
            LED_off();
            a[0] = '0';
        }
    }
    shell_led_state(argv, argc);
}
