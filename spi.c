/**
 * @file spi.c
 * @brief spi procedures
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/stm32/gpio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bool.h"
#include "hw.h"
#include "spi.h"
#include "strings_local.h"
#include "utils.h"

/**
 * main spi struct
 */
spi_state_t spimain;

/**
 * @brief init softspi struct and gpio
 * @param dff - data size, FALSE - 8bit, TRUE - 16bit
 * @param tx  - tx or rx mode, TRUE for tz
 * @param lsbfirst - FALSE - msbfirst
 * @param cpol - clock polarity (1 for 1 in idle)
 * @param cpha - clock phase (1 for falling edge capture)
 * @param sck_port, sck_pin - clock pin definition, mandatory
 * @param mosi_port, mosi_pin - MOSI pin definition
 * @param miso_port, miso_pin - MISO pin definition
 * @param ss_port, ss_pin - SS pin definition
 *
 * MOSI, MISO, SS pins may be swithed off by 255 value in _pin parameter
 */
boolean softspi_init(   spi_state_t *spi,
                        boolean dff,
                        boolean tx,
                        boolean lsbfirst,
                        uint8_t cpol,
                        uint8_t cpha,
                        uint32_t sck_port,
                        uint16_t sck_pin,
                        uint32_t mosi_port,
                        uint16_t mosi_pin,
                        uint32_t miso_port,
                        uint16_t miso_pin,
                        uint32_t ss_port,
                        uint16_t ss_pin)
{
    if (
        (cpol > 1) ||
        (cpha > 1)
        ) { return FALSE; }
    spi->dff = dff;
    spi->tx = tx;
    spi->cpol = cpol;
    spi->cpha = cpha;
    if (sck_pin == 255)
    { return FALSE; } // pin MUST be configured
    else
    {
        spi->sck_pin = sck_pin; spi->sck_port = sck_port;
        config_gpio_output(sck_port, sck_pin);
        if (cpol == 1)
        {
            gpio_set(sck_port, sck_pin);
        }
        else
        {
            gpio_clear(sck_port, sck_pin);
        }
    }
    if (mosi_pin == 255)
    { spi->mosi_pin = 255; }
    else
    {
        spi->mosi_pin = mosi_pin; spi->mosi_port = mosi_port;
        config_gpio_output(mosi_port, mosi_pin);
    }
    if (miso_pin == 255)
    { spi->miso_pin = 255; }
    else
    {
        spi->miso_pin = miso_pin; spi->miso_port = miso_port;
        config_gpio_input(miso_port, miso_pin);
    }
    if (ss_pin == 255)
    { spi->ss_pin = 255; }
    else
    {
        spi->ss_pin = ss_pin; spi->ss_port = ss_port;
        config_gpio_output(ss_port, ss_pin);
        gpio_set(ss_port, ss_pin);
    }
    spi->lsbfirst = lsbfirst;
    spi->drne = FALSE;
    spi->status = SPI_idle;
    return TRUE;
}

/**
 * @brief soft spi fsm procedure
 *
 * This function must be called by timer interrupt
 */
void softspi_fsm(spi_state_t *spi)
{
    uint16_t bit;

    // tx stage
    if (spi->tx && (spi->mosi_pin != 255))
    {
        switch (spi->status)
        {
            // wait for new data
            case SPI_idle:
            {
                if (spi->drne == TRUE) // new data for tx
                {
                    spi->bit_curr = 0;
                    spi->status = SPI_tx_bit_stage1;
                }
                else
                {   // idle sck
                    if (spi->cpol == 0)
                    {
                        gpio_clear(spi->sck_port, spi->sck_pin);
                    }
                    else
                    {
                        gpio_set(spi->sck_port, spi->sck_pin);
                    }
                }
                break;
            }
            // set MISO and may be ss output
            case SPI_tx_bit_stage1:
            {
                if ((spi->bit_curr == 0) && (spi->ss_pin != 255))
                {
                    gpio_set(spi->ss_port, spi->ss_pin);
                }
                bit = spi->dr & 1;
                spi->dr = spi->dr >> 1;
                spi->bit_curr++;
                if (bit == 1)
                {
                   gpio_set(spi->mosi_port, spi->mosi_pin);
                }
                else
                {
                   gpio_clear(spi->mosi_port, spi->mosi_pin);
                }
                // begin SCK pulse
                if ((spi->cpha ^ spi->cpol) == 0)
                {
                    gpio_clear(spi->sck_port, spi->sck_pin);
                }
                else
                {
                    gpio_set(spi->sck_port, spi->sck_pin);
                }
                spi->status = SPI_tx_bit_stage2;
                break;
            }
            // SCK clock pulse end
            case SPI_tx_bit_stage2:
            {
                // end SCK pulse
                if ((spi->cpha ^ spi->cpol) == 0)
                {
                    gpio_set(spi->sck_port, spi->sck_pin);
                }
                else
                {
                    gpio_clear(spi->sck_port, spi->sck_pin);
                }
                spi->status = SPI_tx_bit_stage3;
                break;
            }
            // end of bit transfer
            case SPI_tx_bit_stage3:
            {
                uint8_t maxbits = spi->dff?16:8;
                if (spi->bit_curr >= maxbits)
                {
                    if (spi->ss_pin != 255)
                    {
                        gpio_clear(spi->ss_port, spi->ss_pin);
                    }
                    spi->drne = FALSE;
                    spi->status = SPI_idle;
                }
                else
                {
                    spi->status = SPI_tx_bit_stage1;
                }
                break;
            }
            default: // error
            {
                break;
            }
        }
    }
    // rx stage
    else
    {
        spi->status = SPI_error;
        // not implemented
    }
}

/**
 * @brief send data via softspi
 */
void softspi_send(spi_state_t *spi, uint16_t data)
{
    while (spi->drne) { taskYIELD(); }; // wait for idle
    if (!spi->lsbfirst)
    {
        //uint8_t maxbits = spi->dff?16:8;
        data = reverse_bits(data, spi->dff?16:8);
    }
    spi->dr = data;
    spi->drne = TRUE;
}
