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

/* rotary encoder with button */
#define ENCODER_BUTTON_PORT GPIOA
#define ENCODER_BUTTON_PIN  GPIO15
#define ENCODER_QUAD_PORT   GPIOB
#define ENCODER_QUAD_PIN1   GPIO3
#define ENCODER_QUAD_PIN2   GPIO4

/**
 * uart port
 */
#define UART USART1

/**
 * shell will be echo input chars
 */
#define SHELL_ECHO 1

/**
 * display ports definition
 * RST        - PA2
 * DC         - PA3
 * SCK        - PA5 (SPI1 port)
 * SDA (MOSI) - PA7 (SPI1 port)
 */
#define ST7789_RST_PORT GPIOA
#define ST7789_RST_PIN  GPIO2
#define ST7789_DC_PORT  GPIOA
#define ST7789_DC_PIN   GPIO3
/* CS/SS not used in my display - stay undef */
//#define ST7789_CS_PORT
//#define ST7789_CS_PIN
#define ST7789_SPI      SPI1
#define ST7789_SPI_PORT GPIOA
#define ST7789_SCK      GPIO5
#define ST7789_SDA      GPIO7
