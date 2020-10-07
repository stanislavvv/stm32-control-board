/*
lcd2004.h file is a part of stm32Basic project.

Copyright (c) 2020 vitasam

Based on LCD driver:
http://web.alfredstate.edu/faculty/weimandn/programming/
    lcd/ATmega328/LCD_code_gcc_4d.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef HD44780_H_
#define HD44780_H_

#include "config.h"

#define HD44780_PORT        GPIOB
#define HD44780_E           GPIO10
#define HD44780_RS          GPIO11
#define HD44780_D4          GPIO12
#define HD44780_D5          GPIO13
#define HD44780_D6          GPIO14
#define HD44780_D7          GPIO15
#define HD44780_BACKLIGHT   GPIO1

#define HD44780_SCREEN_WIDTH    20
#define HD44780_SCREEN_HEIGHT   4

/* LCD instructions */
#define LCD_CLEAR 0x01 /* Replace all characters with ASCII 'space' */
#define LCD_HOME 0x02 /* Return cursor to first position on first line */
#define LCD_ENTRYMODE 0x06 /* Shift cursor from left to right on read/write */
#define LCD_DISPLAYOFF 0x08 /* Turn display off */
#define LCD_DISPLAYON 0x0C /* Display on, cursor off, don't blink character */
#define LCD_FUNCTIONRESET 0x30 /* Reset the LCD */
#define LCD_FUNCTIONSET4B 0x28 /* 4-bit data, 2-line display, 5 x 7 font */
#define LCD_SETCURSOR 0x80 /* Set cursor position */

/* Display driver Host API functions */
void hd44780_init(void);
void hd44780_set_cursor(uint8_t col, uint8_t row);
void hd44780_write_character_4d(char theCharacter);
void hd44780_backlight_off(void);
void hd44780_backlight_on(void);
void hd44780_backlight_toggle(void);

/* "Private functions" */
void hd44780_init_gpio(void);
void hd44780_init_4bit_mode(void);
void hd44780_write_string_4d(const char *theString);
void hd44780_clear(void);
void hd44780_home(void);
void hd44780_off(void);
void hd44780_on(void);

#endif
