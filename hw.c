/** @weakgroup hardware
 *  @{
 */
/**
 * @file hw.c
 * @brief hardware interface functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include "config_hw.h"
#include "bool.h"
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/timer.h>
#include <errno.h>
#include "FreeRTOS.h"
#include "task.h"
#include "strings_local.h"
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
 * @brief send named number in human-readable binary
 * @param name - name (max char[10])
 * @param data - sending number up to uint32_t
 * @param nibbles - size of data in nibbles, 1..8
 */
void send_named_bin(char name[], uint32_t data, uint8_t nibbles)
{
    char sendbuffer[50];
    uint8_t c = 0;
    uint8_t n = (uint8_t)strlen_local(name);
    if (n > 10) { n = 10; };
    for (uint8_t i = 0; i < n; i++)
    {
        sendbuffer[c] = name[i];
        c++;
    }
    sendbuffer[c] = ':'; c++;
    sendbuffer[c] = ' '; c++;
    i2bin(data, &sendbuffer[c], nibbles);
    c = (uint8_t)strlen_local(sendbuffer);
    sendbuffer[c] = '\r'; c++;
    sendbuffer[c] = '\n'; c++;
    sendbuffer[c] = 0; c++;
    send_string(sendbuffer);
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
                return ETIME;
            }
        }
    }
    return 0;
}


/**
 * @brief set gpio and other hardware modes
 */
void init_gpio(void)
{
    /* switch to quartz 8MHz + pll 72MHz */
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"

    /* periferial clock */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(UART_RCC);
    rcc_periph_clock_enable(ST7789_RCC);

    /* LED on PC13 */
    gpio_set_mode(
        LED_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        LED_PIN);
    /* led is on after default 0 on pin reset - switch off */
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
    usart_set_baudrate(UART, UART_SPEED);
    usart_set_databits(UART, 8);
    usart_set_stopbits(UART, USART_STOPBITS_1);
    usart_set_parity(UART, USART_PARITY_NONE);
    usart_set_flow_control(UART, USART_FLOWCONTROL_NONE);
    usart_set_mode(UART, USART_MODE_TX_RX);

    /* enable uart */
    usart_enable(UART);

#if BOOT_VERBOSE==1
    send_string("uart initalized\r\n");
#endif

    /* SCK, MOSI(SDA) */
    gpio_set_mode(ST7789_SPI_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, ST7789_SCK | ST7789_SDA);

#if BOOT_VERBOSE==1
    send_string("spi gpio initalized\r\n");
#endif


    /*
     * SPI Registers:
     * CR1:
     *
     * BIDIMODE = 1 (bidirectional MOSI on master)
     * BIDIOE = 1 (transmit-only)
     * CRCEN = 0
     * CRCNEXT = 0
     *
     * DFF = 0 (8-bit))
     * RXONLY = 0
     * SSM = 1
     * SSI = 1 (as SSM)
     *
     * LSBFIRST = 0 (MSB first)
     * SPE = 0 (SPI enable) -- switch to 1 at the end of init
     * BR[2:0] = 101 (fpclk/64)
     * MSTR = 1 (spi master)
     * CPOL = 1 (clock polarity 1 when idle)
     * CPHA = 1 (second clock transition is the first data capture)
     *
     * CR2:
     *
     * TXEIE = 0 (tx empty buf int disabled)
     * RXNEIE = 0 (rx non-empty buf int disabled)
     * ERRIE = 0 (error int disable)
     * [4:3] - reserved
     * SSOE = 0 (SS output disabled)
     * TXDMA = 0 (tx dma disabled)
     * RXDMA = 0 (rx dma disabled)
     *
     * I2SCFGR:
     *
     * all zero - i2s off
     */
    spi_reset(ST7789_SPI);
    uint32_t reg_cr1;
    uint32_t reg_cr2 = 0;
    uint32_t reg_i2scfgr = 0;
    reg_cr1 = (
            SPI_CR1_BIDIMODE |
            SPI_CR1_BIDIOE |
            SPI_CR1_SSM |
            SPI_CR1_SSI |
            SPI_CR1_BAUDRATE_FPCLK_DIV_256 |
            SPI_CR1_MSTR |
            SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE |
            SPI_CR1_CPHA_CLK_TRANSITION_2
          );
    SPI_I2SCFGR(ST7789_SPI) = reg_i2scfgr;
    SPI_CR2(ST7789_SPI) = reg_cr2;
    SPI_CR1(ST7789_SPI) = reg_cr1;

#if BOOT_VERBOSE==1
    send_string("spi initalization:\r\n");
    send_named_bin("CR1 config", reg_cr1, 4);
    send_named_bin("CR1 actial", SPI_CR1(ST7789_SPI), 4);
#endif

    spi_enable(ST7789_SPI);

#if BOOT_VERBOSE==1
    send_string("spi enable:\r\n");
    send_named_bin("CR1 actual", SPI_CR1(ST7789_SPI), 4);
    send_string("hw init end\r\n");
#endif
//#endif // 0

}

/** @}*/
