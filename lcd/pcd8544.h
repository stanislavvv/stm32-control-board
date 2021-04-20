/** @weakgroup lcd
 *  @{
 */
/**
 * @file
 * @brief nokia lcd screen driver definitions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 * adapted from https://github.com/Matiasus/PCD8544
 *
 */
#ifndef PCD8544_H_
#define PCD8544_H_

#ifdef LCD_SPI

#include "libs/bool.h"

/// delay between bytes - parameter for delay_nop()
#define PCD8544_BYTE_DELAY 50

/// number of byte rows on screen
#define PCD8544_MAX_NUM_ROWS 6

/// number of byte columns on screen
#define PCD8544_MAX_NUM_COLS 84

/// size of screen cache
#define PCD8544_CACHE_SIZE_MEM (PCD8544_MAX_NUM_ROWS * PCD8544_MAX_NUM_COLS)

/// font width
#define FONT_WIDTH 5

/// font 5x8, ascii-only
extern const char PCD8544_font5x8[][FONT_WIDTH];

/**
 * @brief send command char to lcd PCD8544
 * @param data - command data
 */
void PCD8544_send_cmd(char data);

/**
 * @brief send data char to lcd PCD8544
 * @param data
 */
void PCD8544_send_data(char data);

/**
 * @brief clear screen cache
 */
void PCD8544_clear(void);

/**
 * @brief send lcd cache to hardware lcd PCD8544
 */
void PCD8544_update(void);

/**
 * @brief put char to lcd screen cache
 * @param c - character
 * @return TRUE - success, FALSE - char not in font, or position not in range
 */
boolean PCD8544_putc(char c);

/**
 * @brief print string to screen cache
 * @param str - string for printing
 */
void PCD8544_print(char *str);

/**
 * @brief Set text position
 * @param x - position / 0 <= rows <= 5
 * @param y - position / 0 <= cols <= 14
 */
char PCD8544_text_setpos(char x, char y);

/**
 * @brief set current pixel position
 * @param x,y - coordinates on screen
 * @return  TRUE - success, FALSE - position out of range
 */
char PCD8544_px_setpos(uint16_t x, uint16_t y);

/**
 * @brief set pixel on coordinates
 * @param x,y - coordinates
 * @return TRUE - success, FALSE - x,y out of range
 */
char PCD8544_px_on(uint16_t x, uint16_t y);

/**
 * @brief clear pixel on coordinates
 * @param x,y - coordinates
 * @return TRUE - success, FALSE - x,y out of range
 */
char PCD8544_px_off(uint16_t x, uint16_t y);

/**
 * @brief PCD8544 lcd test
 */
void PCD8544_test(void);

#endif // ifdef LCD_SPI

/**
 * @brief lcd PCD8544 init
 */
void PCD8544_init(void);

#endif

/** @}*/
