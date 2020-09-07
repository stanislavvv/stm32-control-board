/**
 * @file spi.h
 * @brief spi procedures
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#ifndef SPI_H_

#define SPI_H_

#include "bool.h"

typedef enum // spi fsm states
{
    SPI_idle,
    SPI_tx_bit_stage1,
    SPI_tx_bit_stage2,
    SPI_tx_bit_stage3,
    SPI_tx_bit_stage4,
    SPI_error
} spi_status_t;

/**
 * @brief softspi struct
 */
typedef struct /// spi fsm state, data and config
{
    // state and data
    spi_status_t status; // fsm current state
    uint16_t     bit_curr; // current output/input bit
    uint16_t     dr; // data "register"
    boolean      drne; // FALSE - dr is empty (sent/not received)

    // spi configuration
    boolean  dff; // data size: FALSE - 8bit, TRUE - 16bit
    boolean  tx; // mode: FALSE - rx, TRUE - tx
    boolean  lsbfirst; // FALSE - MSB first
    uint8_t  cpol; // clock polarity when idle (0|1)
    uint8_t  cpha; // clock phase, 0 - on rising, 1 - on falling

    // spi pinout configuration
    uint32_t sck_port;
    uint16_t  sck_pin;
    uint32_t mosi_port;
    uint16_t  mosi_pin;
    uint32_t miso_port;
    uint16_t  miso_pin;
    uint32_t ss_port;
    uint16_t  ss_pin;
} spi_state_t;

extern spi_state_t spimain;

/**
 * @brief init softspi struct and gpio
 * @param dff - data size, 0 - 8bit, 1 - 16bit
 * @param tx  - tx or rx mode, TRUE for tz
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
                        uint16_t ss_pin);

/**
 * @brief soft spi fsm procedure
 *
 * This function must be called by timer interrupt
 */
void softspi_fsm(spi_state_t *spi);

/**
 * @brief send data to spi
 */
void softspi_send(spi_state_t *spi, uint16_t data);

#endif // SPI_H_
