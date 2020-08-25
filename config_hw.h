/* copyright https://github.com/stanislavvv/stm32-control-board */

/* hardware definitions for STM32F103 */

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
