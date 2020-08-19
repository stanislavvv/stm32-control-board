/* copyright  */
/* hardware interaction procedures */
#include "FreeRTOS.h"
#include "task.h"
#include "hw.h"
#include "shell.h"

/* shell processing task */
void task_process_shell(void *args __attribute((unused)))
{
    if (char_is_recv())
    {
        unsigned char c = recv_char();
        send_char(c);
    } else
    {
        taskYIELD();
    }
}
