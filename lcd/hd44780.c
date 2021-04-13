/** @weakgroup lcd
 *  @{
 */
/**
 * @file
 * @brief hd44780 lcd screen driver with 4-bit interface
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 * LCD_* constants and some ideas taken from
 * https://github.com/SayidHosseini/STM32LiquidCrystal/blob/master/src/LiquidCrystal.h
 */

#include <libopencm3/stm32/gpio.h>
#include "libs/bool.h"
#include "hw.h"  // for delay and DBG
#include "config.h"
#include "hd44780.h"

// flags for hd44780_* functions
#define HD44780_COMMAND TRUE  ///< hd44780_puts: indicate command byte
#define HD44780_DATA    FALSE ///< hd44780_puts: indicate data byte

#define HD44780_DISPLAY_ON  TRUE  ///< hd44780_display: display on
#define HD44780_DISPLAY_OFF FALSE ///< hd44780_display: display off

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#if HD44780_SCREEN_HEIGHT==1
uint8_t display_function =
    LCD_4BITMODE |
    LCD_1LINE |
    LCD_5x8DOTS;
#else
uint8_t display_function =
    LCD_4BITMODE |
    LCD_2LINE |
    LCD_5x8DOTS;
#endif

/// send command to LCD
#define COMMAND(d) hd44780_putc(d, HD44780_COMMAND)

/// send data to LCD
#define WRITE(d) hd44780_putc(d, HD44780_DATA)

/// current display state
uint8_t display_control = LCD_CURSOROFF | LCD_BLINKOFF;

/// print left-to-right
uint8_t display_mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

/// display row offsets
uint8_t row_offsets[4] = {0x00, 0x40, 0x00 + HD44780_SCREEN_WIDTH, 0x40 + HD44780_SCREEN_WIDTH};

/**
 * @brief (internal) write 4 most bits to LCD
 * @param data
 * @todo set proper and shorter delay in pulse
 */
static void hd44780_send4(uint8_t data)
{
    if (data & 0x80)
    {
        gpio_set(HD44780_PORT, HD44780_D7);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D7);
    }
    if (data & 0x40)
    {
        gpio_set(HD44780_PORT, HD44780_D6);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D6);
    }
    if (data & 0x20)
    {
        gpio_set(HD44780_PORT, HD44780_D5);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D5);
    }
    if (data & 0x10)
    {
        gpio_set(HD44780_PORT, HD44780_D4);
    }
    else
    {
        gpio_clear(HD44780_PORT, HD44780_D4);
    }

    // E pin pulse
    gpio_set(HD44780_PORT, HD44780_E);
    delay_ms(1); // min 450ns
    gpio_clear(HD44780_PORT, HD44780_E);
    delay_ms(1); // min 37us
}

/**
 * @brief send byte to LCD
 * @param data - byte for sending
 * @param cmd - FALSE if character, TRUE if command
 */
static void hd44780_putc(uint8_t data, boolean cmd)
{
    if (cmd)
    { // command
        gpio_clear(HD44780_PORT, HD44780_RS);
    }
    else
    { // character
        gpio_set(HD44780_PORT, HD44780_RS);
    }
    gpio_clear(HD44780_PORT, HD44780_E);
    hd44780_send4(data);
    hd44780_send4((uint8_t)(data<<4));
}

/**
 * @brief init lcd-related hardware
 */
void hd44780_init(void)
{
    gpio_set_mode(
        HD44780_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
#ifdef HD44780_BACKLIGHT
        HD44780_BACKLIGHT |
#endif
        HD44780_E |
        HD44780_RS |
        HD44780_D4 |
        HD44780_D5 |
        HD44780_D6 |
        HD44780_D7
        );
    delay_ms(50); // min 40ms after powerup
    gpio_clear(HD44780_PORT, HD44780_E);
    gpio_clear(HD44780_PORT, HD44780_RS);

    // init 4-bit interface
    COMMAND(0x3);
    delay_ms(5); // min 4.3ms
    COMMAND(0x3);
    delay_ms(1); // min 100us
    COMMAND(0x3);
    delay_ms(1); // min 100us
    COMMAND(0x2);
    delay_ms(1); // min 100us

    COMMAND(LCD_FUNCTIONSET | display_function);
    delay_ms(1); // min 53us

    hd44780_display(HD44780_DISPLAY_ON);

    hd44780_clear();

    // re-init left-to-right mode
    display_mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    COMMAND(display_mode | LCD_ENTRYMODESET);
}

/**
 * @brief move cursor to coordinates
 * @todo implement it
 */
void hd44780_goto(uint8_t col, uint8_t row)
{}

/**
 * @brief write string to LCD
 * @param s string for writing to display
 */
void hd44780_write(const char s[])
{
    uint16_t i = 0;
    while (s[i] != 0)
    {
        WRITE(s[i]);
        i++;
        ///@todo proper delay
        delay_ms(1); // min 40us
    }
}

/**
 * @brief clear display and set cursor to home
 */
void hd44780_clear(void)
{
    COMMAND(LCD_CLEARDISPLAY);
    ///@todo proper delay
    delay_ms(3);
}

/**
 * @brief set cursor to home
 */
void hd44780_home(void)
{
    COMMAND(LCD_RETURNHOME);
    ///@todo proper delay
    delay_ms(2);
}

/**
 * @brief switch display on or off
 * @param on - FALSE will switch display off, else on
 */
void hd44780_display(boolean on)
{
    if (on)
    {
        display_control |= LCD_DISPLAYON;
    }
    else
    {
        display_control = (uint8_t)(display_control & (~LCD_DISPLAYON));
    }
    COMMAND(display_control | LCD_DISPLAYCONTROL);
    ///@todo proper delay
    delay_ms(1); // min 53us
}

/**
 * @brief set custom CGRAM char shape
 * @param char_id - 0..7
 * @param char_data - array[8] of char data
 */
void hd44780_createchar(uint8_t char_id, uint8_t char_data[])
{
  char_id &= 0x7; // char_id to 0..7
  COMMAND((uint8_t)(LCD_SETCGRAMADDR | (char_id << 3)));
  for (int i=0; i<8; i++)
  {
    WRITE(char_data[i]);
  }
}

///@}
