/** @weakgroup graphics
 *  @{
 */
/**
 * @file hw_lcd.h
 * @brief lowlevel-related shell commands and functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#ifndef HW_LCD_H_
#define HW_LCD_H_

#include "config.h"
#include "hw.h"

#if LCD_TYPE==7789  // lcd st7789

    #include "st7789.h"

    #define LCD_HEIGHT ST7789_HEIGHT ///< height of LCD in pixels
    #define LCD_WIDTH  ST7789_WIDTH  ///< width of LCD in pixels
    #define LCD_INIT() ST7789_Init() ///< LCD init procedure
    #define LCD_TEST() ST7789_Test() ///< LCD demo procedure

    /**
     * @brief set pixel procedure
     * @param x,y - pixel coordinates
     * @param color - color of pixel (16bit)
     */
    #define set_pixel(x, y, color) ST7789_DrawPixel(x, y, color)


#elif LCD_TYPE==8544  // lcd nokia

#include "pcd8544.h"

    // 8 pix in row
    #define LCD_HEIGHT PCD8544_MAX_NUM_ROWS*8 ///< height of LCD in pixels
    #define LCD_WIDTH  PCD8544_MAX_NUM_COLS  ///< width of LCD in pixels
    #define LCD_INIT() PCD8544_init() ///< LCD init procedure
    #define LCD_TEST() PCD8544_test() ///< LCD demo procedure

    /**
     * @brief set pixel procedure
     * @param x,y - pixel coordinates
     * @param color - color of pixel, 0 - clear pixel, other - set
     */
    static inline void set_pixel(uint16_t x, uint16_t y, uint16_t color)
    {
        if (color != 0)
        {
            PCD8544_px_on(x, y);
        }
        else
        {
            PCD8544_px_off(x, y);
        }
    }

#endif

#ifdef LCD_TYPE
/**
 * @brief draw line on lcd screen cache
 * @param x0,y0 - line begin coordinates
 * @param x1,y1 - line end coordinates
 * @param color
 *
 * source: https://www.thecrazyprogrammer.com/2017/01/bresenhams-line-drawing-algorithm-c-c.html
 */
static inline void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t dx, dy, p, x, y;

    dx=(int16_t)(x1-x0);
    dy=(int16_t)(y1-y0);

    x=x0;
    y=y0;

    p=(int16_t)(2*dy-dx);

    while (x<x1)
    {
        if (p>=0)
        {
            set_pixel((uint16_t)x, (uint16_t)y, color);
            y=(int16_t)(y+1);
            p=(int16_t)(p+2*dy-2*dx);
        }
        else
        {
            set_pixel((uint16_t)x, (uint16_t)y, color);
            p=(int16_t)(p+2*dy);
        }
        x=(int16_t)(x+1);
    }
}

/**
 * @brief utility function for circle draw
 * @param xc,yc - center coordinates
 * @param x,y - coordinates delta from center
 * @param color - color of circle
 */
static inline void draw_8px(int16_t xc, int16_t yc, int16_t x, int16_t y, uint16_t color)
{
    set_pixel((uint16_t)(xc+x), (uint16_t)(yc+y), color);
    set_pixel((uint16_t)(xc-x), (uint16_t)(yc+y), color);
    set_pixel((uint16_t)(xc+x), (uint16_t)(yc-y), color);
    set_pixel((uint16_t)(xc-x), (uint16_t)(yc-y), color);
    set_pixel((uint16_t)(xc+y), (uint16_t)(yc+x), color);
    set_pixel((uint16_t)(xc-y), (uint16_t)(yc+x), color);
    set_pixel((uint16_t)(xc+y), (uint16_t)(yc-x), color);
    set_pixel((uint16_t)(xc-y), (uint16_t)(yc-x), color);
}

/**
 * @brief bresenham circle implementation
 */
static inline void draw_circle(int16_t xc, int16_t yc, int16_t r, uint16_t color)
{
    int16_t x = 0, y = r;
    int16_t d = (int16_t)(3 - 2 * r);
    draw_8px(xc, yc, x, y, color);
    while (y >= x)
    {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0)
        {
            y--;
            d = (int16_t)(d + 4 * (x - y) + 10);
        }
        else
            d = (int16_t)(d + 4 * x + 6);
        draw_8px(xc, yc, x, y, color);
    }
}
#endif // ifdef LCD_TYPE

/**
 * @brief lcd test shell command
 * @param argv, argc 'test' will be test spi transfer
 */
static inline void shell_lcd_cmd(char* argv[], uint16_t argc)
{
    (void)(argv);
    (void)(argc);
    send_string("lcd test... ");
    LCD_TEST();
    send_string("end\r\n");
}

#endif // ifndef HW_LCD_H_

/** @}*/
