/**
 * @file config_hw.h
 * @brief hardware configuration definitions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

/* hardware definitions for STM32F103 now */

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
 * rotary encoder with button pins
 * @{
 */
#define ENCODER_BUTTON_PORT GPIOA
#define ENCODER_BUTTON_PIN  GPIO15
#define ENCODER_QUAD_PORT   GPIOB
#define ENCODER_QUAD_PIN1   GPIO3
#define ENCODER_QUAD_PIN2   GPIO4
/**
 * @}
 */

/**
 * uart port
 */
#define UART USART1
#define UART_RCC RCC_USART1
#define UART_SPEED 921600 // max speed on some usb-uart converters

/**
 * shell will be echo input chars
 */
#define SHELL_ECHO 1

/**
 * initalization messages to uart
 */
#define BOOT_VERBOSE 1

/**
 * display ports definition
 * RST        - PA2
 * DC         - PA3
 * SCK        - PA5 (SPI1 port)
 * SDA (MOSI) - PA7 (SPI1 port)
 */
#define LCD_RST_PORT GPIOA
#define LCD_RST_PIN  GPIO2
#define LCD_DC_PORT  GPIOA
#define LCD_DC_PIN   GPIO3

// SPI1

#define LCD_RCC      RCC_SPI1
#define LCD_SPI      SPI1
#define LCD_SPI_PORT GPIOA
#define LCD_SCK      GPIO5
#define LCD_SDA      GPIO7


// SPI2
/*
#define LCD_RCC      RCC_SPI2
#define LCD_SPI      SPI2
#define LCD_SPI_PORT GPIOB
#define LCD_SCK      GPIO13
#define LCD_SDA      GPIO14
*/

/* CS/SS not used in my display - stay undef */
//#define LCD_CS_PORT
//#define LCD_CS_PIN
