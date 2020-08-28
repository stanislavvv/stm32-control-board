/**
 * @file hw.c
 * @brief hardware interface functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <errno.h>
#include "FreeRTOS.h"
#include "task.h"
#include "config_hw.h"
#include "hw.h"
#include "bool.h"
#include "strings_local.h"

/**
 * @brief send null-terminated string to uart
 * @param char s[] - string for sending to uart
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
 * @brief send buffer to spi with timeout
 * @param spi  spi port, ex. SPI1 in libopencm3
 * @param buffer  buffer of bytes for sending
 * @param length  length of buffer
 * @param timeout  timeout in ticks, portMAX_DELAY and 0 - switch off
 * @return errno - 0 (ok) or EBUSY (busy), ETIME (timeout), EIO (error)
 *
 * Transmit buffer with given length to spi in two-wire 8-bit mode with timeout
 */
uint16_t spi_send_buffer_2wire_8bit(uint32_t spi, uint8_t *buffer,
                            uint16_t length, TickType_t timeout)
{
    TickType_t tickstart = xTaskGetTickCount();
    uint16_t initial_count = length;

    /* bad parameters */
    if ((buffer == NULL) || (length == 0U))
    {
        return EIO;
    }

    /* transmit to spi now */
    spi_set_bidirectional_transmit_only_mode(spi);

    while (initial_count > 0)
    {
        if (SPI_SR(spi) & SPI_SR_TXE)
        {
            SPI_DR(spi) = (*buffer);
            buffer += sizeof(uint8_t);
            initial_count--;
        }
        else
        {
            /* timeout management */
            if (
                 ((xTaskGetTickCount() - tickstart) >= timeout) &&
                 (timeout != portMAX_DELAY) &&
                 (timeout != (TickType_t)0)
               )
            {
//    send_string("spi_send_buffer_2wire_8bit end ETIME");
                return ETIME;
            }
        }
    }
//    send_string("spi_send_buffer_2wire_8bit end success");
    return 0;
}


/**
 * @brief set gpio and other hardware modes
 * @param none
 * @return none
 */
void init_gpio(void)
{
    /* switch to quartz 8MHz + pll 72MHz */
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"

    /* periferial clock */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(UART_RCC);
    rcc_periph_clock_enable(ST7789_RCC);

    /* LED on PC13 */
    gpio_set_mode(
        LED_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        LED_PIN);
    /* led is on after pin init - switch off */
    LED_off();

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

    /* uart tx PA9 */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
    /* uart rx PA10 */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

    /* setup uart parameters */
    /* usart_set_baudrate(UART, 115200); */
    usart_set_baudrate(UART, 921600);
    usart_set_databits(UART, 8);
    usart_set_stopbits(UART, USART_STOPBITS_1);
    usart_set_parity(UART, USART_PARITY_NONE);
    usart_set_flow_control(UART, USART_FLOWCONTROL_NONE);
    usart_set_mode(UART, USART_MODE_TX_RX);

    /* enable uart */
    usart_enable(UART);

    /* LCD on SPI */
    /* DC pin */
    gpio_set_mode(ST7789_DC_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        ST7789_DC_PIN);
    /* RST pin */
    gpio_set_mode(ST7789_RST_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        ST7789_RST_PIN);

    /* SCK, MOSI(SDA) */
    gpio_set_mode(ST7789_SPI_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, ST7789_SCK | ST7789_SDA);

//  /*
//   * According the STM32 Datasheet for SPI peripheral we need to PULLDOWN
//   * or PULLUP the SCK pin according the polarity used.
//   */
//  pull = (handle->Init.CLKPolarity == SPI_POLARITY_LOW) ? GPIO_PULLDOWN : GPIO_PULLUP;
//  pin_PullConfig(get_GPIO_Port(STM_PORT(obj->pin_sclk)), STM_LL_GPIO_PIN(obj->pin_sclk), pull);
//  pinmap_pinout(obj->pin_ssel, PinMap_SPI_SSEL);

    /* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
    spi_reset(ST7789_SPI);
// 3.3V ST7789 display that has no CS pin.   It needs SPI mode#3.
    /* Set up SPI in Master mode with:
    * Clock baud rate: 1/64 of peripheral clock frequency
    * Clock polarity: Idle High
    * Clock phase: Data valid on 1nd clock pulse
    * Data frame format: 8-bit
    * Frame format: MSB First
    */
    spi_init_master(ST7789_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    /*
    * Set NSS management to software.
    *
    * Note:
    * Setting nss high is very important, even if we are controlling the GPIO
    * ourselves this bit needs to be at least set to 1, otherwise the spi
    * peripheral will not send any data out.
    */
    spi_enable_software_slave_management(ST7789_SPI);
    spi_set_nss_high(ST7789_SPI);

    /* bidirectional spi - use only MOSI for data transfer */
    spi_set_bidirectional_mode(ST7789_SPI);

    /* Enable ST7789_SPI periph. */
    spi_enable(ST7789_SPI);

    /* init by register access */
    //uint32_t reg32 = SPI_CR1(ST7789_SPI);
    /* Reset all bits omitting SPE, CRCEN and CRCNEXT bits. */
    //reg32 &= SPI_CR1_SPE | SPI_CR1_CRCEN | SPI_CR1_CRCNEXT;
    //reg32 |= (uint32_t)SPI_CR1_MSTR;                    /* Configure SPI as master. */
    //reg32 |= (uint32_t)SPI_CR1_BAUDRATE_FPCLK_DIV_64;   /* Set baud rate bits. */
    //reg32 |= (uint32_t)SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE; /* Set CPOL value. */
    //reg32 |= (uint32_t)SPI_CR1_CPHA_CLK_TRANSITION_1;   /* Set CPHA value. */
    //reg32 |= (uint32_t)SPI_CR1_DFF_8BIT;                /* Set data format (8 or 16 bits). */
    //reg32 |= (uint32_t)SPI_CR1_MSBFIRST;                /* Set frame format (LSB- or MSB-first). */
    //reg32 |= (uint32_t)SPI_CR1_SSM;                     /* Set software slave management */
    //reg32 |= (uint32_t)SPI_CR1_BIDIMODE;                /* Set bidirectional mode */
    //reg32 |= (uint32_t)SPI_CR1_BIDIOE;                  /* Set bidirectional transmit-only mode */
    //reg32 &= (uint32_t)(~SPI_CR1_CRCEN);                /* Set no CRC */
    //reg32 |= (uint32_t)SPI_CR1_SPE;                     /* enable SPI */
    //reg32 &= (uint32_t)(~SPI_CR1_CRCNEXT);              /* transmit data, not crc*/

    //SPI_CR1(ST7789_SPI) = reg32;
    //SPI_CR2(ST7789_SPI) &= (uint32_t)(~SPI_CR2_SSOE);     /* allow slave select to be an input */
}
