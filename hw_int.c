/** @weakgroup hardware
 *  @{
 */

/**
 * @file hw_int.c
 * @brief interrupt vector procedures (most of them for debug)
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include "FreeRTOS.h"
#include "rtos/queue.h"
#include "bool.h"
#include "hw.h"

/*void wwdg_isr(void)
{
    send_string("wwdg_isr int\r\n");
    while (1) { };
}
void pvd_isr(void)
{
    send_string("pvd_isr int\r\n");
    while (1) { };
}
void tamper_isr(void)
{
    send_string("tamper_isr int\r\n");
    while (1) { };
}
void rtc_isr(void)
{
    send_string("rtc_isr int\r\n");
    while (1) { };
}
void flash_isr(void)
{
    send_string("flash_isr int\r\n");
    while (1) { };
}
void rcc_isr(void)
{
    send_string("flash_isr int\r\n");
    while (1) { };
}
void exti0_isr(void)
{
    send_string("exti0_isr int\r\n");
    while (1) { };
}
void exti1_isr(void)
{
    send_string("exti1_isr int\r\n");
    while (1) { };
}
void exti2_isr(void)
{
    send_string("exti2_isr int\r\n");
    while (1) { };
}
void exti3_isr(void)
{
    send_string("exti3_isr int\r\n");
    while (1) { };
}
void exti4_isr(void)
{
    send_string("exti4_isr int\r\n");
    while (1) { };
}
void dma1_channel1_isr(void)
{
    send_string("dma1-1_isr int\r\n");
    while (1) { };
}
void dma1_channel2_isr(void)
{
    send_string("dma1-2_isr int\r\n");
    while (1) { };
}
void dma1_channel3_isr(void)
{
    send_string("dma1-3_isr int\r\n");
    while (1) { };
}
void dma1_channel4_isr(void)
{
    send_string("dm1-4_isr int\r\n");
    while (1) { };
}
void dma1_channel5_isr(void)
{
    send_string("dma1-5_isr int\r\n");
    while (1) { };
}
void dma1_channel6_isr(void)
{
    send_string("dma1-6_isr int\r\n");
    while (1) { };
}
void dma1_channel7_isr(void)
{
    send_string("dma1-7_isr int\r\n");
    while (1) { };
}
void adc1_2_isr(void)
{
    send_string("adc1_2_isr int\r\n");
    while (1) { };
}
void usb_hp_can_tx_isr(void)
{
    send_string("usb_hp_can_tx_isr int\r\n");
    while (1) { };
}
void usb_lp_can_rx0_isr(void)
{
    send_string("usb_lp_can_rx0_isr int\r\n");
    while (1) { };
}
void can_rx1_isr(void)
{
    send_string("can_rx1_isr int\r\n");
    while (1) { };
}
void can_sce_isr(void)
{
    send_string("can_sce_isr int\r\n");
    while (1) { };
}
void exti9_5_isr(void)
{
    send_string("exti9_5_isr int\r\n");
    while (1) { };
}
void tim1_brk_isr(void)
{
    send_string("tim1_brk_isr int\r\n");
    while (1) { };
}
void tim1_up_isr(void)
{
    send_string("tim1_up_isr int\r\n");
    while (1) { };
}
void tim1_trg_com_isr(void)
{
    send_string("tim1_trg_isr int\r\n");
    while (1) { };
}
void tim1_cc_isr(void)
{
    send_string("tim1_cc_isr int\r\n");
    while (1) { };
}
void tim2_isr(void)
{
    send_string("tim2_isr int\r\n");
    while (1) { };
}
void tim3_isr(void)
{
    send_string("tim3_isr int\r\n");
    while (1) { };
}
void i2c1_ev_isr(void)
{
    send_string("i2c1_ev_isr int\r\n");
    while (1) { };
}
void i2c1_er_isr(void)
{
    send_string("i2c1_er_isr int\r\n");
    while (1) { };
}
void i2c2_ev_isr(void)
{
    send_string("i2c2_ev_isr int\r\n");
    while (1) { };
}
void i2c2_er_isr(void)
{
    send_string("i2c2_er_isr int\r\n");
    while (1) { };
}
void spi1_isr(void)
{
    send_string("spi1_isr int\r\n");
    while (1) { };
}

void spi2_isr(void)
{
    send_string("spi2_isr int\r\n");
    while (1) { };
}
void usart1_isr(void)
{
    send_string("usart1_isr int\r\n");
    while (1) { };
}
void usart2_isr(void)
{
    send_string("usart2_isr int\r\n");
    while (1) { };
}
void usart3_isr(void)
{
    send_string("usart3_isr int\r\n");
    while (1) { };
}
void exti15_10_isr(void)
{
    send_string("exti15_10_isr int\r\n");
    while (1) { };
}*/
//void rtc_alarm_isr(void)
//void usb_wakeup_isr(void)
//void tim8_brk_isr(void)
//void tim8_up_isr(void)
//void tim8_trg_com_isr(void)
//void tim8_cc_isr(void)
//void adc3_isr(void)
//void fsmc_isr(void)
//void sdio_isr(void)
//void tim5_isr(void)
//void spi3_isr(void)
//void uart4_isr(void)
//void uart5_isr(void)
//void tim6_isr(void)
//void tim7_isr(void)
//void dma2_channel1_isr(void)
//void dma2_channel2_isr(void)
//void dma2_channel3_isr(void)
//void dma2_channel4_5_isr(void)
//void dma2_channel5_isr(void)
//void eth_isr(void)
//void eth_wkup_isr(void)
//void can2_tx_isr(void)
//void can2_rx0_isr(void)
//void can2_rx1_isr(void)
//void can2_sce_isr(void)
//void otg_fs_isr(void)
void hard_fault_handler(void)
{
    send_string("--- hard_fault_handler int ---\r\n");
    while (1) { };
}

/*
#include "spi.h"
uint8_t tim4_isr_flag = 0;
uint8_t tim4_data = 0;
void tim4_isr(void)
{
    gpio_set(LED_PORT, LED_PIN);
    softspi_fsm(&spimain);
    gpio_clear(LED_PORT, LED_PIN);

    //debug
    // clear interrupt flag
    TIM_SR(TIM4) &= ~TIM_SR_UIF;
}
*/

/** @}*/
