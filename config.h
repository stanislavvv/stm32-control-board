/**
 * @file
 * @brief firmware configuration definitions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

/// shell will be echo input chars
#define SHELL_ECHO 1

/// enable initalization messages to uart
#define BOOT_VERBOSE 1

/// enable debug print to uart
#define DEBUG_PRINT 1

/// hardware or software spi
#define HW_SPI 1

/// onboard blue pill led port
#define LED_PORT            GPIOC

/// onboard blue pill led pin
#define LED_PIN             GPIO13

/// onboard blue pill led pin number
#define LED_PIN_NUM         13

/**
 * @brief rotary encoder with button pins
 * @{
 */
#define ENCODER_BUTTON_PORT GPIOA  ///< encoder button port
#define ENCODER_BUTTON_PIN  GPIO15 ///< encoder button pin
#define ENCODER_QUAD_PORT   GPIOB  ///< encoder quadrature port
#define ENCODER_QUAD_PIN1   GPIO3  ///< encoder quadrature pin 1
#define ENCODER_QUAD_PIN2   GPIO4  ///< encoder quadrature pin 2
/// @}

/**
 * @brief uart port definitions
 * @{
 */
#define UART USART1         ///< uart device
#define UART_RCC RCC_USART1 ///< uart clock
#define UART_SPEED 921600   ///< uart speed. 921600 max speed on some usb-uart converters
/// @}

/**
 * @brief type of LCD
 *
 * Posible values:
 * - 7789 for ST7789 (16/18bit color)
 * - 8544 for PCD8544 (nokia screen)
 * - 44780 for text lcd (hd44780-based)
 */
//#define LCD_TYPE 7789 // st7789
//#define LCD_TYPE 8544 // nokia lcd
#define LCD_TYPE 44780 // text lcd

#if LCD_TYPE==44780
    // textmode hd44780-based LCD, 4-bit mode
    #define HD44780_PORT      GPIOB
    // not used in module
    // #define HD44780_RW        GPIO9
    #define HD44780_E         GPIO10
    #define HD44780_RS        GPIO11
    #define HD44780_D4        GPIO12
    #define HD44780_D5        GPIO13
    #define HD44780_D6        GPIO14
    #define HD44780_D7        GPIO15
    // not implemented
    // #define HD44780_BACKLIGHT GPIO8

    #define HD44780_SCREEN_WIDTH  16
    #define HD44780_SCREEN_HEIGHT 2
#endif

#if LCD_TYPE==7789
    #define LCD_CLK_POL SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE ///< LCD SPI clock polarity
    #define LCD_CLK_PHA SPI_CR1_CPHA_CLK_TRANSITION_2   ///< LCD SPI clock phase
#endif

#if LCD_TYPE==8544
    #define LCD_CLK_POL SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE ///< LCD SPI clock polarity
    #define LCD_CLK_PHA SPI_CR1_CPHA_CLK_TRANSITION_1   ///< LCD SPI clock phase
    #define LCD_CS_PORT  GPIOA  ///< LCD CS(SS) port
    #define LCD_CS_PIN   GPIO6  ///< LCD CS(SS) pin
#endif

#if ( (LCD_TYPE==7789) || (LCD_TYPE==8544) ) // graphic LCD
    #define LCD_BYTEORDER SPI_CR1_MSBFIRST

    #define LCD_RST_PORT GPIOA ///< LCD RST port
    #define LCD_RST_PIN  GPIO3 ///< LCD RST pin
    #define LCD_DC_PORT  GPIOA ///< LCD DC port
    #define LCD_DC_PIN   GPIO4 ///< LCD DC pin

    // SPI1
    #define LCD_RCC      RCC_SPI1 ///< LCD SPI clock
    #define LCD_SPI      SPI1     ///< LCD SPI device
    #define LCD_SPI_PORT GPIOA    ///< LCD SPI port
    #define LCD_SCK      GPIO5    ///< LCD SPI SCK pin
    #define LCD_SDA      GPIO7    ///< LCD SPI SDA(MOSI) pin


    /*
    // SPI2
    #define LCD_RCC      RCC_SPI2 ///< LCD SPI clock
    #define LCD_SPI      SPI2     ///< LCD SPI device
    #define LCD_SPI_PORT GPIOB    ///< LCD SPI device
    #define LCD_SCK      GPIO13   ///< LCD SPI SCK pin
    #define LCD_SDA      GPIO14   ///< LCD SPI SDA(MOSI) pin
    */
#endif // graphic LCD
