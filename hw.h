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
#include "FreeRTOS.h"
#include "task.h"
#include "config.h"
#include "libs/bool.h"

#if DEBUG==1
    /// debug print
    //static inline void DBG(char*s) { send_string(s); }
    #define DBG(s) { send_string(s); }
#else
    #define DBG(s)
#endif // DEBUG

/// switch led on
#define LED_on() gpio_clear(LED_PORT, LED_PIN)

/// switch led off
#define LED_off() gpio_set(LED_PORT, LED_PIN)

/// get led state
#define LED_state() (GPIO_ODR(LED_PORT) && LED_PIN)


#include "hw/uart.h"


/**
 * @brief init hardware related stuff
 */
void init_hw(void);

/**
 * @brief delay to given time in ms
 * @param ms  time in milliseconds up to 65535
 */
static inline void delay_ms(uint16_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief delay for critical section (without rtos functions)
 * @param dly delay value
 */
static inline void delay_nop(uint32_t dly)
{
    ///@todo calibrate this delay in us/ms
    for (uint32_t i = 0; i < dly; i++)    /* Wait a bit. */
        { __asm__("nop"); }
}

#endif // ifdef HW_H_
/// @}
