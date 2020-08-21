/* copyright  */
#include "FreeRTOS.h"
#include "task.h"
#include "config_hw.h"
#include "hw.h"
#include <libopencm3/stm32/usart.h>

/* receive char from uart */
unsigned char recv_char(void)
{
    return (unsigned char) (0xff & usart_recv_blocking(UART));
}

/* send char to uart */
void send_char(unsigned char c)
{
    usart_send_blocking(UART, (uint16_t)(c));
}

/* char is received */
uint16_t char_is_recv(void)
{
    /* STM32F1 specific */
    return (USART_SR(UART) & USART_SR_RXNE) == 0;
}

void init_gpio(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz(); // For "blue pill"
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_USART1);

    /* LED on PC13 */
    gpio_set_mode(
        LED_PORT,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_PUSHPULL,
        LED_PIN);

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
        GPIO_CNF_INPUT_FLOAT, GPIO_USART1_TX);

    /* setup uart parameters */
    usart_set_baudrate(UART, 115200);
    usart_set_databits(UART, 8);
    usart_set_stopbits(UART, USART_STOPBITS_1);
    usart_set_mode(UART, USART_MODE_TX_RX);
    usart_set_parity(UART, USART_PARITY_NONE);
    usart_set_flow_control(UART, USART_FLOWCONTROL_NONE);

    /* enable uart */
    usart_enable(UART);
}
