/**
 * @file
 * @brief firmware configuration definitions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

/**
 * onboard blue pill led port
 */
#define LED_PORT            GPIOC

/**
 * onboard blue pill led pin
 */
#define LED_PIN             GPIO13

/**
 * onboard blue pill led pin number
 */
#define LED_PIN_NUM         13

/**
 * @brief rotary encoder with button pins
 */
#define ENCODER_BUTTON_PORT GPIOA  ///< encoder button port
#define ENCODER_BUTTON_PIN  GPIO15 ///< encoder button pin
#define ENCODER_QUAD_PORT   GPIOB  ///< encoder quadrature port
#define ENCODER_QUAD_PIN1   GPIO3  ///< encoder quadrature pin 1
#define ENCODER_QUAD_PIN2   GPIO4  ///< encoder quadrature pin 2

/**
 * uart port definitions
 */
#define UART USART1         ///< uart device
#define UART_RCC RCC_USART1 ///< uart clock
#define UART_SPEED 921600   ///< uart speed. 921600 max speed on some usb-uart converters

/**
 * shell will be echo input chars
 */
#define SHELL_ECHO 1

/**
 * enable initalization messages to uart
 */
#define BOOT_VERBOSE 1

/**
 * type of LCD
 *
 * Posible values:
 * - 7789 for ST7789 (16/18bit color)
 * - 8544 for PCD8544 (nokia screen)
 */
#define LCD_TYPE 7789 // st7789
//#define LCD_TYPE 8544 // nokia lcd

#define LCD_RST_PORT GPIOA ///< LCD RST port
#define LCD_RST_PIN  GPIO2 ///< LCD RST pin
#define LCD_DC_PORT  GPIOA ///< LCD DC port
#define LCD_DC_PIN   GPIO3 ///< LCD DC pin

// SPI1

#define LCD_RCC      RCC_SPI1 ///< LCD SPI clock
#define LCD_SPI      SPI1     ///< LCD SPI device
#define LCD_SPI_PORT GPIOA    ///< LCD SPI port
#define LCD_SCK      GPIO5    ///< LCD SPI SCK pin
#define LCD_SDA      GPIO7    ///< LCD SPI SDA(MOSI) pin


// SPI2
/*
#define LCD_RCC      RCC_SPI2 ///< LCD SPI clock
#define LCD_SPI      SPI2     ///< LCD SPI device
#define LCD_SPI_PORT GPIOB    ///< LCD SPI device
#define LCD_SCK      GPIO13   ///< LCD SPI SCK pin
#define LCD_SDA      GPIO14   ///< LCD SPI SDA(MOSI) pin
*/

/* comment them, if CS/SS not used */
#define LCD_CS_PORT  GPIOA  ///< LCD CS(SS) port
#define LCD_CS_PIN   GPIO4  ///< LCD CS(SS) pin
///@}
