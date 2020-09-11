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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "config.h"
#include "hw.h"

/**
 * @brief send null-terminated string to uart
 * @param s[] - string for sending to uart
 * @return none
 */
void send_string(const char s[])
{
    uint16_t i = 0;
    while (s[i] != 0)
    {
        send_char(s[i]);
        i++;
    }
}

/**
 * @brief init clock
 */
static inline void init_rcc(void)
{
    /* switch to quartz 8MHz + pll 72MHz */
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"

    /* periferial clock */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(UART_RCC);
    rcc_periph_clock_enable(LCD_RCC);
}

/**
 * @brief set generic gpio pin modes
 */
static inline void init_gpio(void)
{
        /* LED on PC13 */
    gpio_set_mode(
        LED_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        LED_PIN);
    /* led is on after default 0 on pin reset - switch off */
    LED_off();

    // encoder init for software polling
    /* encoder button on PA15 */
    gpio_set_mode(
        ENCODER_BUTTON_PORT,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_PULL_UPDOWN,
        ENCODER_BUTTON_PIN);
    /* pull up button */
    uint32_t port = GPIO_ODR(GPIOA);
    GPIO_BSRR(GPIOA) = (port & GPIO15);

    /* encoder quadrature on PB4,PB5 */
    gpio_set_mode(
        ENCODER_QUAD_PORT,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT,
        ENCODER_QUAD_PIN1|ENCODER_QUAD_PIN2);
}

/**
 * @brief init uart
 */
static inline void init_uart(void)
{
        /* uart tx PA9 */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
    /* uart rx PA10 */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

    /* setup uart parameters */
    usart_set_baudrate(UART, UART_SPEED);
    usart_set_databits(UART, 8);
    usart_set_stopbits(UART, USART_STOPBITS_1);
    usart_set_parity(UART, USART_PARITY_NONE);
    usart_set_flow_control(UART, USART_FLOWCONTROL_NONE);
    usart_set_mode(UART, USART_MODE_TX_RX);

    /* enable uart */
    usart_enable(UART);
}

/**
 * @brief init hardware related stuff
 */
void init_hw(void)
{
    init_rcc();
    init_gpio();
    init_uart();
#if BOOT_VERBOSE==1
    send_string("uart initalized\r\n");
#endif
}



/// @}
