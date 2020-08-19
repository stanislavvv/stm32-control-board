/* copyright  */
#include "FreeRTOS.h"
#include "task.h"

/* receive char from uart */
unsigned char recv_char(void);

/* send char to uart */
void send_char(unsigned char c);

/* char is received */
uint16_t char_is_recv(void);
