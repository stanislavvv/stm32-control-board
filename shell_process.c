/* copyright https://github.com/stanislavvv/stm32-control-board */

#include "shell_process.h"

/*
 *
 * name: shell_process
 * @param  outbuffer  output buffer zero-terminated string
 * @param  command_line  command from user input, zero-terminated string
 *
 */
void shell_process(char outbuffer[], char command_line[])
{
    /* simply send 'processed' and copy first word ([^ ]) */
    char pr[13] = "Processed: ";
    int i;
    i = 0;
    for (i = 0; pr[i] != 0; i++)
    {
        outbuffer[i] = pr[i];
    }
    int l = i;
    for (i = 0 ; command_line[i] != ' ' && i < 256; i++)
    {
        outbuffer[l] = command_line[i];
        l++;
    }
}
