#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static inline void init_gpio(void) {
    rcc_clock_setup_pll(&rcc_hse8mhz_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(
        GPIOC, GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE, GPIO13);
    gpio_set_output_options(
        GPIOC,
        GPIO_OTYPE_PP,
        GPIO_OSPEED_50MHZ,
        GPIO13);
}
