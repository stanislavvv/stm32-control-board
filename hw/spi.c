/** @weakgroup hardware
 *  @{
 */
/**
 * @file
 * @brief spi interface functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "config.h"
#include "bool.h"
#include "hw/spi.h"
#include "hw.h"

#if HW_SPI==0 // soft "spi"

/* * <- space fix docs generation bug
 * @brief shift out byte to port with clock
 * @param bitOrder - 1 - LSB first, 0 - MSB first
 * @param val - data to shift
 *
 * adapted from wiring shiftOut
 */
void shiftOut_lcd(uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    taskENTER_CRITICAL();
    for (i = 0; i < 8; i++)
    {
        if (bitOrder == 1)
        {
            if (val & 1)
            {
                gpio_set(LCD_SPI_PORT, LCD_SDA);
            }
            else
            {
                gpio_clear(LCD_SPI_PORT, LCD_SDA);
            }
            val >>= 1;
        }
        else
        {
            if ((val & 128) != 0)
            {
                gpio_set(LCD_SPI_PORT, LCD_SDA);
            }
            else
            {
                gpio_clear(LCD_SPI_PORT, LCD_SDA);
            }
            val <<= 1;
        }
        delay_nop(10);
        gpio_set(LCD_SPI_PORT, LCD_SCK);
        delay_nop(20);
        gpio_clear(LCD_SPI_PORT, LCD_SCK);
        delay_nop(10);
    }
    taskEXIT_CRITICAL();
}

#endif

/* *
 * @brief send buffer to spi with timeout
 * @param spi  spi port, ex. SPI1 in libopencm3
 * @param buffer  buffer of bytes for sending
 * @param length  length of buffer
 * @param timeout  timeout in ticks, portMAX_DELAY and 0 - switch off
 * @return errno - TRUE ok, FALSE - error (bad parameters or timeout)
 *
 * Transmit buffer with given length to spi in two-wire 8-bit mode with timeout
 */
boolean spi_send_buffer_2wire_8bit(uint32_t spi, uint8_t *buffer,
                            uint16_t length, TickType_t timeout)
{
    TickType_t tickstart = xTaskGetTickCount();
    uint16_t initial_count = length;

    /* check bad parameters */
    if ((buffer == NULL) || (length == 0U))
    {
        return FALSE;
    }

    while (initial_count > 0)
    {
        if (SPI_TX_READY(spi))
        {
            SPI_WRITE(spi, (*buffer));
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
                return FALSE;
            }
        }
    }
    return TRUE;
}

/**
 * @brief init spi
 *
 * init spi with parameters from config.h
 */
void init_spi(void)
{
    /* SCK, MOSI(SDA) */
#if HW_SPI==1
    rcc_periph_clock_enable(LCD_RCC);
    gpio_set_mode(LCD_SPI_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, LCD_SCK | LCD_SDA);

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
     * CPOL = 0/1 (clock polarity 1 when idle)
     * CPHA = 0/1 (second clock transition is the first data capture)
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
    spi_reset(LCD_SPI);
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
            LCD_CLK_POL |
            LCD_CLK_PHA
          );
    SPI_I2SCFGR(LCD_SPI) = reg_i2scfgr;
    SPI_CR2(LCD_SPI) = reg_cr2;
    SPI_CR1(LCD_SPI) = reg_cr1;

    spi_enable(LCD_SPI);

// softspi
#else
    gpio_set_mode(LCD_SPI_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, LCD_SCK | LCD_SDA);
#endif

    /* RST */
    gpio_set_mode(LCD_RST_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, LCD_RST_PIN);
    gpio_set(LCD_RST_PORT, LCD_RST_PIN); // RST inactive

    /* DC */
    gpio_set_mode(LCD_DC_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, LCD_DC_PIN);

#ifdef LCD_CS_PORT
    /* CS/SS controlled by software */
    gpio_set_mode(LCD_CS_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, LCD_CS_PIN);
    gpio_set(LCD_CS_PORT, LCD_CS_PIN); // CS inactive
#endif
}

///@}
