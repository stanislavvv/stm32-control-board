/* copyright https://github.com/stanislavvv/stm32-control-board */


#ifndef SHELL_PROCESS_H_

#include <stdint.h>
//#include "shell_hw.h"

#define SHELL_PROCESS_H_

#define SHELL_MAX_CLI_LENGTH 64
#define SHELL_MAX_OUT_LENGTH 256
#define SHELL_MAX_ARGS 4

/* shell input buffer */
extern char shell_input_buffer[SHELL_MAX_CLI_LENGTH];

/* shell input buffer length */
extern uint16_t shell_in_lastchar;

/* shell output buffer */
extern char shell_output_buffer[SHELL_MAX_OUT_LENGTH];

/* shell output buffer length */
extern uint16_t shell_out_lastchar;

/*
 *
 * name: shell_process
 * @param  outbuffer  output buffer zero-terminated string
 * @param  command_line  command from user input, zero-terminated string
 *
 */
void shell_process(void);

/*
 *
 * name: shell_in_buffer_add
 * @param char c - received character
 * @return bool - non-true on overflow
 *
 */
uint16_t shell_in_buffer_add(char c);

/* add string to output buffer */
void shell_out_buffer_add(char s[]);

/*
 *
 * name: shell_cleanup_output
 *
 * clean shell output buffer
 */
void shell_cleanup_output(void);

#endif
