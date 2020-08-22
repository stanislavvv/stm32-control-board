/* copyright https://github.com/stanislavvv/stm32-control-board */

#include <stdint.h>
#include "shell_process.h"

/* shell input buffer */
char shell_input_buffer[SHELL_MAX_CLI_LENGTH] = "\0";

/* shell input buffer length */
uint16_t shell_in_lastchar = 0;

/* shell output buffer */
char shell_output_buffer[SHELL_MAX_OUT_LENGTH] = "\0";

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

/*
 *
 * name: shell_buffer_add
 * @param char c - received character
 * @return bool - non-true on overflow
 *
 */
uint16_t shell_buffer_add(char c)
{
    if (shell_in_lastchar >= SHELL_MAX_CLI_LENGTH) {
        return 1;
    } else
    {
        shell_input_buffer[shell_in_lastchar] = c;
        shell_in_lastchar++;
        shell_input_buffer[shell_in_lastchar] = '\0';
        return 0;
    }
}
