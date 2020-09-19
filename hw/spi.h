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

#include <libopencm3/stm32/spi.h>
#include "hw.h"

#if HW_SPI==1
    /**
     * @brief write to spi macro or function
     * @param spi - for example SPI1
     * @param c - byte for writing
     */
    static inline void SPI_WRITE(uint32_t spi, uint8_t c) { spi_write(spi, c); }

    /**
     * @brief write to spi with blocking macro or function
     * @param spi - for example SPI1
     * @param c - byte for writing
     */
    static inline void SPI_SEND(uint32_t spi, uint8_t c) { spi_send(spi, c); }

    /**
     * @brief return true if spi ready for tx new data
     * @param spi for example SPI1
     */
    static inline uint16_t SPI_TX_READY(uint32_t spi) { return (SPI_SR(spi) & SPI_SR_TXE); }

#else // HW_SPI==0
    /**
     * @brief shift out byte to port with clock
     * @param bitOrder - 1 - LSB first, 0 - MSB first
     * @param val - byte data to shift
     *
     * adapted from wiring shiftOut
     */
    void shiftOut_lcd(uint8_t bitOrder, uint8_t val);

    #define SPI_WRITE(spi, c) shiftOut_lcd(0, c)
    #define SPI_SEND(spi, c) shiftOut_lcd(0, c)
    // always true in soft spi
    #define SPI_TX_READY(spi) TRUE

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

/**
 * @brief show spi registers and may test spi transfer
 * @param argv, argc 'test' will be test spi transfer
 */
void shell_spi_cmd(char* argv[], uint16_t argc);

/**
 * @brief init spi
 *
 * init spi with parameters from config.h
 */
void init_spi(void);


///@}
