/*
hd44780.c file is a part of stm32Basic project.

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
//#include <stdio.h>
//#include <string.h>
#include "hw.h"
#include "hd44780.h"
#include "config.h"

const char displayName[] = "HD44780 20x4";

void hd44780_init(void)
{
    hd44780_init_gpio();
    hd44780_init_4bit_mode();
    hd44780_clear();
    hd44780_home();
}


void hd44780_init_gpio(void)
{
    // Set digital output functions for LCD pins
    gpio_set_mode(
        HD44780_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        HD44780_E | HD44780_RS |
        HD44780_D4 | HD44780_D5| HD44780_D6 | HD44780_D7 |
        HD44780_BACKLIGHT);
}


/** @brief Send a byte of data to the LCD module
 *  (private function).
 *  @note RS is configured for the desired LCD register output to
 *  begin at the new position. If the cursor is
 *  currently hidden, a call to set_cursor() must not show
 *  the cursor. E is low. RW is low.
 *  @param theByte is a data to be sent to the desired LCD register.
 *  @return None.
 */
static void hd44780_write_4(uint8_t theByte)
{
    ///@todo fix delays to lower than 1ms
    if (theByte & (1 << 7))
    {
        gpio_set(HD44780_PORT, HD44780_D7);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D7);
    }

    if (theByte & (1 << 6))
    {
        gpio_set(HD44780_PORT, HD44780_D6);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D6);
    }

    if (theByte & (1 << 5))
    {
        gpio_set(HD44780_PORT, HD44780_D5);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D5);
    }

    if (theByte & (1 << 4))
    {
        gpio_set(HD44780_PORT, HD44780_D4);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D4);
    }

    /* Write the data. 'Address set-up time' (40 nS) */
    gpio_set(HD44780_PORT, HD44780_E);

    /* 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS) */
    delay_ms(1);
    gpio_clear(HD44780_PORT, HD44780_E);

    /* 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS) */
    delay_ms(1);
}

/** @brief Send a byte of data to the LCD instruction register.
 *  (private function).
 *  @note Does not deal with RW (busy flag is not implemented).
 *  @param theByte is a data to be sent to LCD instruction register.
 *  @return None.
 */
static void hd44780_write_instruction_4d(uint8_t theInstruction)
{
    gpio_clear(HD44780_PORT, HD44780_RS);
    gpio_clear(HD44780_PORT, HD44780_E); /* Make sure E is initially low */
    hd44780_write_4(theInstruction); /* Write the upper 4-bits */
    hd44780_write_4(theInstruction << 4); /* Write the lower 4-bits */
}

/** @brief Initialize the LCD module for a 4-bit data interface.
 *  @return None.
 */
void hd44780_init_4bit_mode()
{
    /* Power-up delay, at least 40 msec */
    delay_ms(100); /* 100 msec */

    /*
    IMPORTANT - At this point the LCD module is in the 8-bit mode
    and it is expecting to receive 8 bits of data, one bit on each of
    its 8 data lines, each time the 'E' line is pulsed.

    Since the LCD module is wired for the 4-bit mode, only the
    upper four data lines are connected to the microprocessor and the
    lower four data lines are typically left open. Therefore, when
    the 'E' line is pulsed, the LCD controller will read whatever data
    has been set up on the upper four data lines and the lower four
    data lines will be high (due to internal pull-up circuitry).

    Fortunately the 'LCD_FUNCTIONRESET' instruction does not care
    about what is on the lower four bits so this instruction can be sent
    on just the four available data lines and it will be interpreted
    properly by the LCD controller. The 'hd44780_write_4' subroutine will
    accomplish this if the control lines have previously been configured
    properly.
    */

    /* Set up the RS and E lines for the 'hd44780_write_4' subroutine. */
    gpio_clear(HD44780_PORT, HD44780_RS);
    gpio_clear(HD44780_PORT, HD44780_E); /* Make sure E is initially low */

    /* Reset the LCD controller */
    hd44780_write_4(LCD_FUNCTIONRESET); /* First part of reset sequence */
    delay_ms(5); /* 4.1 mS delay (min) */

    /* The 2nd part of reset sequence */
    hd44780_write_4(LCD_FUNCTIONRESET);
    delay_ms(1); /* 100uS delay (min) */

    /* The 3rd part of reset sequence */
    hd44780_write_4(LCD_FUNCTIONRESET);
    delay_ms(1); /* this delay is omitted in the data sheet */

    /*
    Preliminary Function Set instruction - used only to set the 4-bit mode.
    The number of lines or the font cannot be set at this time since the
    controller is still in the 8-bit mode, but the data transfer mode can be
    changed since this parameter is determined by one of the upper four
    bits of the instruction.
    */

    hd44780_write_4(LCD_FUNCTIONSET4B); /* Set 4-bit mode */
    delay_ms(1); /* 40uS delay (min) */

    /* Function Set instruction */
    hd44780_write_instruction_4d(LCD_FUNCTIONSET4B); /* Set mode, lines, font */
    delay_ms(1); /* 40uS delay (min) */

    /*
    The next three instructions are specified in the data sheet as part of
    the initialization routine, so it is a good idea (but probably not
    necessary) to do them just as specified and then redo them later if
    the application requires a different configuration.
    */

    /* Display On/Off Control instruction */
    hd44780_write_instruction_4d(LCD_DISPLAYOFF); /* Turn display OFF */
    delay_ms(1); /* 40uS delay (min) */

    /* Clear Display instruction */
    hd44780_write_instruction_4d(LCD_CLEAR); /* Clear display RAM */
    delay_ms(2); /* 1.64 mS delay (min) */

    /* Entry Mode Set instruction */
    hd44780_write_instruction_4d(LCD_ENTRYMODE); /* Set shift characteristics */
    delay_ms(1); /* 40uS delay (min) */

    /*
    This is the end of the LCD controller initialization as specified in
    the data sheet, but the display has been left in the OFF condition.
    This is a good time to turn the display back ON.
    */

    /* Display On/Off Control instruction */
    hd44780_write_instruction_4d(LCD_DISPLAYON); /* Turn the display ON */
    delay_ms(1); /* 40uS delay (min) */
}

/** @brief Send a byte of data to the LCD data register.
 *  @note Does not deal with RW (busy flag is not implemented).
 *  @param theCharacter is a data to be sent to LCD data register.
 *  @return None.
 */
void hd44780_write_character_4d(char theCharacter)
{
    gpio_set(HD44780_PORT, HD44780_RS);
    gpio_clear(HD44780_PORT, HD44780_E); /* Make sure E is initially low */
    hd44780_write_4((uint8_t)theCharacter); /* Write the upper 4-bits */
    hd44780_write_4((uint8_t)theCharacter << 4); /* Write the lower 4-bits */
}

/** @brief Display a string of characters on the LCD.
 *  @note Uses time delays rather than checking the busy flag.
 *  @param theString is the string to be displayed.
 *  @return None.
 */
void hd44780_write_string_4d(const char *theString)
{
    volatile uint16_t i = 0;
    while (theString[i] != 0)
    {
        hd44780_write_character_4d((uint8_t)theString[i]);
        i++;
        delay_ms(1); /* 40uS delay (min) */
    }
}

/** @brief Clear display RAM.
 *  @param None.
 *  @return None.
 */
void hd44780_clear(void)
{
    hd44780_write_instruction_4d(LCD_CLEAR);
    delay_ms(2); /* 1.64 mS delay (min) */
}

/** @brief Set cursor position to zero.
 *  @param None.
 *  @return None.
 */
void hd44780_home(void)
{
    hd44780_write_instruction_4d(LCD_HOME);
    delay_ms(2); /* 1.64 mS delay (min) */
}

/** @brief Set cursor position to Column and Row.
 *  @param col is start position of line of the LCD (0..19).
 *  @param row is start line of the LCD (0..3).
 *  @return None.
 */
void hd44780_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};

    hd44780_write_instruction_4d(LCD_SETCURSOR | (col + row_offsets[row]));
    delay_ms(2); /* 1.64 mS delay (min) */
}

/** @brief Set display OFF.
 *  @param None.
 *  @return None.
 */
void hd44780_off(void)
{
    hd44780_write_instruction_4d(LCD_DISPLAYOFF);
    delay_ms(2); /* 1.64 mS delay (min) */
}

/** @brief Set display ON.
 *  @param None.
 *  @return None.
 */
void hd44780_on(void)
{
    hd44780_write_instruction_4d(LCD_DISPLAYON);
    delay_ms(2); /* 1.64 mS delay (min) */
}

/** @brief Set display's backlight OFF.
 *  @param None.
 *  @return None.
 */
void hd44780_backlight_off(void)
{
    gpio_clear(HD44780_PORT, HD44780_BACKLIGHT);
}

/** @brief Set display's backlight ON.
 *  @param None.
 *  @return None.
 */
void hd44780_backlight_on(void)
{
    gpio_set(HD44780_PORT, HD44780_BACKLIGHT);
}

/** @brief Toggle display's backlight.
 *  @param None.
 *  @return None.
 */
void hd44780_backlight_toggle(void)
{
    gpio_toggle(HD44780_PORT, HD44780_BACKLIGHT);
}
