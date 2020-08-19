/* copyright  */
#include "FreeRTOS.h"
#include "task.h"
#include "init_hw.h"
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
    return (USART_SR(UART) & USART_SR_RXNE) == 0;
}
