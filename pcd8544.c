/** @weakgroup graphics
 *  @{
 */
/**
 * @file
 * @brief nokia lcd screen driver
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 * adapted from https://github.com/Matiasus/PCD8544
 *
 */

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "bool.h"
#include "config.h"
#include "hw.h"
#include "pcd8544.h"

/// font 5x8, ascii-only
const char PCD8544_font5x8[][FONT_WIDTH] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 }, // 20 space
    { 0x00, 0x00, 0x5f, 0x00, 0x00 }, // 21 !
    { 0x00, 0x07, 0x00, 0x07, 0x00 }, // 22 "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 }, // 23 #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 }, // 24 $
    { 0x23, 0x13, 0x08, 0x64, 0x62 }, // 25 %
    { 0x36, 0x49, 0x55, 0x22, 0x50 }, // 26 &
    { 0x00, 0x05, 0x03, 0x00, 0x00 }, // 27 '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 }, // 28 (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 }, // 29 )
    { 0x14, 0x08, 0x3e, 0x08, 0x14 }, // 2a *
    { 0x08, 0x08, 0x3e, 0x08, 0x08 }, // 2b +
    { 0x00, 0x50, 0x30, 0x00, 0x00 }, // 2c ,
    { 0x08, 0x08, 0x08, 0x08, 0x08 }, // 2d -
    { 0x00, 0x60, 0x60, 0x00, 0x00 }, // 2e .
    { 0x20, 0x10, 0x08, 0x04, 0x02 }, // 2f /
    { 0x3e, 0x51, 0x49, 0x45, 0x3e }, // 30 0
    { 0x00, 0x42, 0x7f, 0x40, 0x00 }, // 31 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 }, // 32 2
    { 0x21, 0x41, 0x45, 0x4b, 0x31 }, // 33 3
    { 0x18, 0x14, 0x12, 0x7f, 0x10 }, // 34 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 }, // 35 5
    { 0x3c, 0x4a, 0x49, 0x49, 0x30 }, // 36 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 }, // 37 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 }, // 38 8
    { 0x06, 0x49, 0x49, 0x29, 0x1e }, // 39 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 }, // 3a :
    { 0x00, 0x56, 0x36, 0x00, 0x00 }, // 3b ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 }, // 3c <
    { 0x14, 0x14, 0x14, 0x14, 0x14 }, // 3d =
    { 0x00, 0x41, 0x22, 0x14, 0x08 }, // 3e >
    { 0x02, 0x01, 0x51, 0x09, 0x06 }, // 3f ?
    { 0x32, 0x49, 0x79, 0x41, 0x3e }, // 40 @
    { 0x7e, 0x11, 0x11, 0x11, 0x7e }, // 41 A
    { 0x7f, 0x49, 0x49, 0x49, 0x36 }, // 42 B
    { 0x3e, 0x41, 0x41, 0x41, 0x22 }, // 43 C
    { 0x7f, 0x41, 0x41, 0x22, 0x1c }, // 44 D
    { 0x7f, 0x49, 0x49, 0x49, 0x41 }, // 45 E
    { 0x7f, 0x09, 0x09, 0x09, 0x01 }, // 46 F
    { 0x3e, 0x41, 0x49, 0x49, 0x7a }, // 47 G
    { 0x7f, 0x08, 0x08, 0x08, 0x7f }, // 48 H
    { 0x00, 0x41, 0x7f, 0x41, 0x00 }, // 49 I
    { 0x20, 0x40, 0x41, 0x3f, 0x01 }, // 4a J
    { 0x7f, 0x08, 0x14, 0x22, 0x41 }, // 4b K
    { 0x7f, 0x40, 0x40, 0x40, 0x40 }, // 4c L
    { 0x7f, 0x02, 0x0c, 0x02, 0x7f }, // 4d M
    { 0x7f, 0x04, 0x08, 0x10, 0x7f }, // 4e N
    { 0x3e, 0x41, 0x41, 0x41, 0x3e }, // 4f O
    { 0x7f, 0x09, 0x09, 0x09, 0x06 }, // 50 P
    { 0x3e, 0x41, 0x51, 0x21, 0x5e }, // 51 Q
    { 0x7f, 0x09, 0x19, 0x29, 0x46 }, // 52 R
    { 0x46, 0x49, 0x49, 0x49, 0x31 }, // 53 S
    { 0x01, 0x01, 0x7f, 0x01, 0x01 }, // 54 T
    { 0x3f, 0x40, 0x40, 0x40, 0x3f }, // 55 U
    { 0x1f, 0x20, 0x40, 0x20, 0x1f }, // 56 V
    { 0x3f, 0x40, 0x38, 0x40, 0x3f }, // 57 W
    { 0x63, 0x14, 0x08, 0x14, 0x63 }, // 58 X
    { 0x07, 0x08, 0x70, 0x08, 0x07 }, // 59 Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 }, // 5a Z
    { 0x00, 0x7f, 0x41, 0x41, 0x00 }, // 5b [
    { 0x02, 0x04, 0x08, 0x10, 0x20 }, // 5c backslash
    { 0x00, 0x41, 0x41, 0x7f, 0x00 }, // 5d ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 }, // 5e ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 }, // 5f _
    { 0x00, 0x01, 0x02, 0x04, 0x00 }, // 60 `
    { 0x20, 0x54, 0x54, 0x54, 0x78 }, // 61 a
    { 0x7f, 0x48, 0x44, 0x44, 0x38 }, // 62 b
    { 0x38, 0x44, 0x44, 0x44, 0x20 }, // 63 c
    { 0x38, 0x44, 0x44, 0x48, 0x7f }, // 64 d
    { 0x38, 0x54, 0x54, 0x54, 0x18 }, // 65 e
    { 0x08, 0x7e, 0x09, 0x01, 0x02 }, // 66 f
    { 0x0c, 0x52, 0x52, 0x52, 0x3e }, // 67 g
    { 0x7f, 0x08, 0x04, 0x04, 0x78 }, // 68 h
    { 0x00, 0x44, 0x7d, 0x40, 0x00 }, // 69 i
    { 0x20, 0x40, 0x44, 0x3d, 0x00 }, // 6a j
    { 0x7f, 0x10, 0x28, 0x44, 0x00 }, // 6b k
    { 0x00, 0x41, 0x7f, 0x40, 0x00 }, // 6c l
    { 0x7c, 0x04, 0x18, 0x04, 0x78 }, // 6d m
    { 0x7c, 0x08, 0x04, 0x04, 0x78 }, // 6e n
    { 0x38, 0x44, 0x44, 0x44, 0x38 }, // 6f o
    { 0x7c, 0x14, 0x14, 0x14, 0x08 }, // 70 p
    { 0x08, 0x14, 0x14, 0x14, 0x7c }, // 71 q
    { 0x7c, 0x08, 0x04, 0x04, 0x08 }, // 72 r
    { 0x48, 0x54, 0x54, 0x54, 0x20 }, // 73 s
    { 0x04, 0x3f, 0x44, 0x40, 0x20 }, // 74 t
    { 0x3c, 0x40, 0x40, 0x20, 0x7c }, // 75 u
    { 0x1c, 0x20, 0x40, 0x20, 0x1c }, // 76 v
    { 0x3c, 0x40, 0x30, 0x40, 0x3c }, // 77 w
    { 0x44, 0x28, 0x10, 0x28, 0x44 }, // 78 x
    { 0x0c, 0x50, 0x50, 0x50, 0x3c }, // 79 y
    { 0x44, 0x64, 0x54, 0x4c, 0x44 }, // 7a z
    { 0x00, 0x08, 0x36, 0x41, 0x00 }, // 7b {
    { 0x00, 0x00, 0x7f, 0x00, 0x00 }, // 7c |
    { 0x00, 0x41, 0x36, 0x08, 0x00 }, // 7d }
    { 0x10, 0x08, 0x08, 0x10, 0x08 }, // 7e ~
    { 0x00, 0x00, 0x00, 0x00, 0x00 }  // 7f
};

/// lcd screen cache (or buffer)
static uint8_t PCD8544_cache[PCD8544_CACHE_SIZE_MEM];

/// lcd screen cache index
uint16_t PCD8544_cache_i = 0;

#ifdef LCD_CS_PORT
    // active low
    #define LCD_SELECT() gpio_clear(LCD_CS_PORT, LCD_CS_PIN)
    #define LCD_UNSELECT() gpio_set(LCD_CS_PORT, LCD_CS_PIN)
#else
    #define LCD_SELECT()
    #define LCD_UNSELECT()
#endif

/**
 * @brief lcd PCD8544 init
 */
void PCD8544_init(void)
{
    gpio_set(LCD_RST_PORT, LCD_RST_PIN);
    PCD8544_reset();

    // extended instruction set
    PCD8544_send_cmd(0x21);

    // bias 1:48 - optimum bias value
    PCD8544_send_cmd(0x13);

    // temperature set - temperature coefficient of IC
    //PCD8544_send_cmd(0x06);

    // for mux 1:48 optimum operation voltage is Ulcd = 6,06.Uth
    // Ulcd = 3,06 + (Ucp6 to Ucp0).0,06
    // 6 < Ulcd < 8,05
    // command for operation voltage = 0x1 Ucp6 Ucp5 Ucp4 Ucp3 Ucp2 Ucp1 Ucp0
    // Ulcd = 0x11000010 = 7,02 V
    PCD8544_send_cmd(0xC2);

    // normal instruction set
    // horizontal adressing mode
    PCD8544_send_cmd(0x20);

    // all display segments on
    PCD8544_send_cmd(0x09);

    // display blank
    PCD8544_send_cmd(0x08);

    // normal mode
    PCD8544_send_cmd(0x0C);
}

/**
 * @brief send command char to lcd PCD8544
 * @param data - command data
 */
void PCD8544_send_cmd(char data)
{
    LCD_SELECT();

    gpio_clear(LCD_DC_PORT, LCD_DC_PIN); // command mode

    spi_send(LCD_SPI, data); // send to lcd
    while (!spi_tx_ready(LCD_SPI)) { }; // wait for send success
//    shiftOut_lcd(0, data);

    LCD_UNSELECT();
}

/**
 * @brief send data char to lcd PCD8544
 * @param data
 */
void PCD8544_send_data(char data)
{
    LCD_SELECT();

    gpio_set(LCD_DC_PORT, LCD_DC_PIN); // data mode

    spi_send(LCD_SPI, data); // send to lcd
    while (!spi_tx_ready(LCD_SPI)) { }; // wait for send success
//    shiftOut_lcd(0, data);

    LCD_UNSELECT();
}

/**
 * @brief send reset impulse to lcd PCD8544
 */
void PCD8544_reset(void)
{
    LCD_SELECT();

//    delay_ms(1); // delay at least 1 ms
    gpio_clear(LCD_RST_PORT, LCD_RST_PIN); // reset
    delay_ms(100);
    gpio_set(LCD_RST_PORT, LCD_RST_PIN);

    LCD_UNSELECT();
}

/**
 * @brief clear screen cache
 */
void PCD8544_clear(void)
{
    for (uint16_t i = 0; i < PCD8544_CACHE_SIZE_MEM; i++)
    {
        PCD8544_cache[i] = 0;
    }
}

/**
 * @brief send lcd cache to hardware lcd PCD8544
 */
void PCD8544_update(void)
{
    uint16_t i;
    PCD8544_text_setpos(0, 0);
    for (i=0; i<PCD8544_CACHE_SIZE_MEM; i++)
    {
        PCD8544_send_data(PCD8544_cache[i]);
    }
}

/**
 * @brief put char to lcd screen cache
 * @param c - character
 * @return TRUE - success, FALSE - char not in font, or position not in range
 */
boolean PCD8544_putc(char c)
{
    uint16_t i;
    // check if c is out of font range
    if ((c < 0x20) ||
        (c > 0x7f)) { return FALSE; }

    // char position cover right screen edge
    if ((PCD8544_cache_i % PCD8544_MAX_NUM_COLS) > (PCD8544_MAX_NUM_COLS - 5))
    {
        // char position not out of range on new line
        if (
             (
                ((PCD8544_cache_i / PCD8544_MAX_NUM_COLS) + 1) * PCD8544_MAX_NUM_COLS
              ) > PCD8544_CACHE_SIZE_MEM
            )
        {
            // out of range
            return FALSE;
        }
        // move char position to new line
        PCD8544_cache_i = (uint16_t)(
                     ((PCD8544_cache_i / PCD8544_MAX_NUM_COLS) + 1) * PCD8544_MAX_NUM_COLS
            );
    }

    // char from font to screen cache
    for (i = 0; i < FONT_WIDTH; i++)
    {
        PCD8544_cache[PCD8544_cache_i++] = PCD8544_font5x8[c - 32][i];
    }
    PCD8544_cache_i++; // one pixel spacing between chars

    return TRUE;
}

/**
 * @brief print string to screen cache
 * @param str - string for printing
 */
void PCD8544_print(char *str)
{
    uint16_t i = 0;
    while (str[i] != '\0')
    {
        PCD8544_putc(str[i++]);
    }
}

/**
 * @brief set next char position
 * @param x - row number
 * @param y - column number
 */
char PCD8544_text_setpos(char x, char y)
{
    // check if x, y is in range
    if ((x >= PCD8544_MAX_NUM_ROWS) ||
        (y >= (PCD8544_MAX_NUM_COLS / (FONT_WIDTH + 1) )))
    { return FALSE; }

    // normal instruction set / horizontal adressing mode
    PCD8544_send_cmd(0x20);

    // set x-position
    PCD8544_send_cmd((0x40 | x));

    // set y-position
    PCD8544_send_cmd((char)( 0x80 | (y * (FONT_WIDTH + 1))) );

    // calculate index memory
    PCD8544_cache_i = (uint16_t)((y * 6) + (x * PCD8544_MAX_NUM_COLS));

    // success return
    return TRUE;
}

/**
 * @brief set current pixel position
 * @param x,y - coordinates on screen
 * @return  TRUE - success, FALSE - position out of range
 */
char PCD8544_px_setpos(uint16_t x, uint16_t y)
{
    // check if x, y is in range
    if ((x >= (PCD8544_MAX_NUM_ROWS * 8)) ||
        (y >=  PCD8544_MAX_NUM_COLS))
    { return FALSE; }

    // normal instruction set
    // horizontal adressing mode
    PCD8544_send_cmd(0x20);

    // set x-position
    PCD8544_send_cmd((char)(0x40 | (x / 8)));

    // set y-position
    PCD8544_send_cmd((char)(0x80 | y));

    // calculate index memory
    PCD8544_cache_i = (uint16_t)(y + ((x / 8) * PCD8544_MAX_NUM_COLS));

    // success return
    return TRUE;
}

/**
 * @brief set pixel on coordinates
 * @param x,y - coordinates
 * @return TRUE - success, FALSE - x,y out of range
 */
char PCD8544_px_on(uint16_t x, uint16_t y)
{
    // set pixel position
    if (0 == PCD8544_px_setpos(x, y))
    { return FALSE; }

    PCD8544_cache[PCD8544_cache_i] |= (char)(1 << (x % 8));

    return TRUE;
}

/**
 * @brief clear pixel on coordinates
 * @param x,y - coordinates
 * @return TRUE - success, FALSE - x,y out of range
 */
char PCD8544_px_off(uint16_t x, uint16_t y)
{
    // set pixel position
    if (0 == PCD8544_px_setpos(x, y))
    { return FALSE; }

    PCD8544_cache[PCD8544_cache_i] &= (char)(~(1 << (x % 8)));

    return TRUE;
}

/**
 * @brief PCD8544 lcd test
 */
void PCD8544_test(void)
{
    PCD8544_init();
    PCD8544_clear();
    PCD8544_print("Hello world!");
    PCD8544_update();
}

/** @}*/
