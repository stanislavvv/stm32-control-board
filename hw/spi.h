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
/**
 * @defgroup spi SPI
 * SPI-related functions and definitions
 */
/**
 * @ingroup spi
 * @{
 */

#ifndef HW_SPI_H_
#define HW_SPI_H_

#ifdef LCD_SPI
#include <libopencm3/stm32/spi.h>
#include "hw.h"

extern uint16_t lcd_clk_pol;
extern uint16_t lcd_clk_pha;

#if HW_SPI==1

    /**
     * @brief write to spi macro or function
     * @param spi - for example SPI1
     * @param c - byte for writing
     */
    #define SPI_WRITE(spi, c) spi_write(spi, c)

    /**
     * @brief write to spi with blocking macro or function
     * @param spi - for example SPI1
     * @param c - byte for writing
     */
    #define SPI_SEND(spi, c) spi_send(spi, c)

    /**
     * @brief return true if spi ready for tx new data
     * @param spi for example SPI1
     */
    #define SPI_TX_READY(spi) (SPI_SR(spi) & SPI_SR_TXE)

    /**
     * @brief return true if spi busy
     */
    #define SPI_BUSY(spi) (SPI_SR(spi) & SPI_SR_BSY)

#else // HW_SPI==0

    /**
     * @brief shift out byte to port with clock
     * @param bitOrder - 1 - LSB first, 0 - MSB first
     * @param val - byte data to shift
     *
     * adapted from wiring shiftOut, softspi realization for 8-bit tx
     */
    void shiftOut_lcd(uint8_t bitOrder, uint8_t val);

    #define SPI_WRITE(spi, c) shiftOut_lcd(LCD_BYTEORDER, c)
    #define SPI_SEND(spi, c) shiftOut_lcd(LCD_BYTEORDER, c)
    // always true in soft spi
    #define SPI_TX_READY(spi) TRUE
    // always false in soft spi
    #define SPI_BUSY(spi) FALSE

#endif // HW_SPI

/**
 * @brief send buffer to spi with timeout
 * @param spi  spi port, ex. SPI1 in libopencm3
 * @param buffer  buffer of **bytes** for sending
 * @param length  length of buffer
 * @param timeout  timeout in ticks, portMAX_DELAY and 0 - switch off
 * @return errno - TRUE ok, FALSE - error (bad parameters or timeout)
 *
 * Transmit buffer with given length to spi in two-wire 8-bit mode with timeout
 */
boolean spi_send_buffer_2wire_8bit(uint32_t spi, uint8_t *buffer,
                            uint16_t length, TickType_t timeout);

#endif // ifdef LCD_SPI

/**
 * @brief dump five spi regs
 *
 * Dump content of five spi regs from LCD_SPI to serial console.
 *
 * Side effects - as from read SPI_SR (clear OVR bit)
 */
void spi_dump_regs(void);

#endif
/**
 * @brief init spi
 *
 * init spi with parameters from config.h
 */
void init_spi(void);
///@}
///@}
