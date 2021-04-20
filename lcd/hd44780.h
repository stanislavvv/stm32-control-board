/** @weakgroup lcd
 *  @{
 */
/**
 * @file
 * @brief hd44780 lcd screen driver with 4-bit interface
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#ifndef HW_HD44780_H_
#define HW_HD44780_H_

#ifdef HD44780_PORT

#include "libs/bool.h"

/**
 * @brief init lcd-related hardware
 */
void hd44780_init(void);

/**
 * @brief move cursor to coordinates
 */
void hd44780_goto(uint8_t col, uint8_t row);

/**
 * @brief write string to LCD
 */
void hd44780_write(const char s[]);

/**
 * @brief clear display and set cursor to home
 */
void hd44780_clear(void);

/**
 * @brief set cursor to home
 */
void hd44780_home(void);

/**
 * @brief switch display on or off
 * @param on - FALSE will switch display off, else on
 */
void hd44780_display(boolean on);

/**
 * @brief set custom CGRAM char shape
 * @param char_id - 0..7
 * @param char_data - array[8] of char data
 */
void hd44780_createchar(uint8_t char_id, uint8_t char_data[]);

#endif // HD44780_PORT

#endif
///@}
