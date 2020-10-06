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
#include "bool.h"
#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "shell.h"
#include "strings_local.h"
#include "hw.h"
#include "hw/spi.h"
#include "hw/uart.h"

/**
 * @brief init all clock
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
    uint32_t port = GPIO_ODR(ENCODER_BUTTON_PORT);
    GPIO_BSRR(ENCODER_BUTTON_PORT) = (port & ENCODER_BUTTON_PIN);

    /* encoder quadrature on PB4,PB5 */
    gpio_set_mode(
        ENCODER_QUAD_PORT,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT,
        ENCODER_QUAD_PIN1|ENCODER_QUAD_PIN2);
    /* pull up quadrature */
    port = GPIO_ODR(ENCODER_QUAD_PORT);
    GPIO_BSRR(ENCODER_QUAD_PORT) = (port & (ENCODER_QUAD_PIN1|ENCODER_QUAD_PIN2));
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

    init_spi();
#if BOOT_VERBOSE==1
    send_string("spi enabled:\r\n");
    send_named_bin("CR1 actual", SPI_CR1(LCD_SPI), 4);
#endif

#if BOOT_VERBOSE==1
    send_string("hw init end\r\n");
#endif
}

/// @}
