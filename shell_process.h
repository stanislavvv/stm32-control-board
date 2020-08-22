/* copyright https://github.com/stanislavvv/stm32-control-board */

#include <stdint.h>

#ifndef SHELL_PROCESS_H_

#define SHELL_PROCESS_H_

#define SHELL_MAX_CLI_LENGTH 256
#define SHELL_MAX_OUT_LENGTH 1024

/* shell input buffer */
extern char shell_input_buffer[SHELL_MAX_CLI_LENGTH];

/* shell input buffer length */
extern uint16_t shell_in_lastchar;

/* shell output buffer */
extern char shell_output_buffer[SHELL_MAX_OUT_LENGTH];

/*
 *
 * name: shell_process
 * @param  outbuffer  output buffer zero-terminated string
 * @param  command_line  command from user input, zero-terminated string
 *
 */
void shell_process(char outbuffer[], char command_string[]);

/*
 *
 * name: shell_buffer_add
 * @param char c - received character
 * @return bool - non-true on overflow
 *
 */
uint16_t shell_buffer_add(char c);

#endif
