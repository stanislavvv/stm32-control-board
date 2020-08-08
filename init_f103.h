#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static inline void init_gpio(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(
        GPIOC,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        GPIO13);
}
