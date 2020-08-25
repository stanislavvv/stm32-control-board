/* copyright https://github.com/stanislavvv/stm32-control-board */
#include "FreeRTOS.h"
#include "task.h"
#include "config_hw.h"
#include "hw.h"
#include "bool.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>


/**
 *
 * name: recv_chars
 * @brief receive char from uart
 * @param none
 * @return received char
 *
 */
char recv_char(void)
{
    return (char) (0xff & usart_recv_blocking(UART));
}

/**
 *
 * name: send_char
 * @brief send char to uart
 * @param char c - char for sending to uart
 * @return none
 *
 */
void send_char(char c)
{
    usart_send_blocking(UART, (uint16_t)(c));
}

/**
 *
 * name: send_string
 * @brief send null-terminated string to uart
 * @param char s[] - string for sending to uart
 * @return none
 *
 */
void send_string(const char s[])
{
    uint16_t i = 0;
    while (s[i] != 0)
    {
        send_char(s[i]);
        i++;
    }
}

/**
 *
 * name: char_is_recv
 * @brief return true if uart has received char in register
 * @param none
 * @return bool char received state
 *
 */
boolean char_is_recv(void)
{
    /* STM32F1 specific */
    return (USART_SR(UART) & USART_SR_RXNE) != 0;
}


/**
 *
 * name: init_gpio
 * @brief set gpio and other hardware modes
 * @param none
 * @return none
 *
 */
void init_gpio(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_SPI1);


    /* LED on PC13 */
    gpio_set_mode(
        LED_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        LED_PIN);
    LED_off();

    /* encoder button on PA15 */
    gpio_set_mode(
        ENCODER_BUTTON_PORT,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_PULL_UPDOWN,
        ENCODER_BUTTON_PIN);

    /* pull up button */
    uint32_t port = GPIO_ODR(GPIOA);
    GPIO_BSRR(GPIOA) = (port & GPIO15);

    /* encoder quadrature on PB4,PB5 */
    gpio_set_mode(
        ENCODER_QUAD_PORT,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT,
        ENCODER_QUAD_PIN1|ENCODER_QUAD_PIN2);

    /* uart tx PA9 */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
    /* uart rx PA10 */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

    /* setup uart parameters */
    usart_set_baudrate(UART, 115200);
    usart_set_databits(UART, 8);
    usart_set_stopbits(UART, USART_STOPBITS_1);
    usart_set_parity(UART, USART_PARITY_NONE);
    usart_set_flow_control(UART, USART_FLOWCONTROL_NONE);
    usart_set_mode(UART, USART_MODE_TX_RX);

    /* enable uart */
    usart_enable(UART);

    /* LCD on SPI */
    /* DC pin */
    gpio_set_mode(ST7789_DC_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        ST7789_DC_PIN);
    /* RST pin */
    gpio_set_mode(ST7789_RST_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        ST7789_RST_PIN);

    /* SCK, MOSI(SDA) */
    gpio_set_mode(ST7789_SPI_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, ST7789_SCK | ST7789_SDA);

    /* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
    spi_reset(ST7789_SPI);

    /* Set up SPI in Master mode with:
    * Clock baud rate: 1/64 of peripheral clock frequency
    * Clock polarity: Idle High
    * Clock phase: Data valid on 1nd clock pulse
    * Data frame format: 8-bit
    * Frame format: MSB First
    */
    spi_init_master(ST7789_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    /*
    * Set NSS management to software.
    *
    * Note:
    * Setting nss high is very important, even if we are controlling the GPIO
    * ourselves this bit needs to be at least set to 1, otherwise the spi
    * peripheral will not send any data out.
    */
    spi_enable_software_slave_management(ST7789_SPI);
    spi_set_nss_high(ST7789_SPI);

    /* unidirectional spi - use only MOSI for data transfer */
    spi_set_unidirectional_mode(ST7789_SPI);

    /* Enable ST7789_SPI periph. */
    spi_enable(ST7789_SPI);
}
