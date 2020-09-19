/** @weakgroup shell
 *  @{
 */
/**
 * @file
 * @brief shell functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

/**
 * @defgroup rtos Rtos-related
 * FreeRTOS-related functions
 */

#ifndef SHELL_H_

#include <stdint.h>
#include "bool.h"

#define SHELL_PROCESS_H_

/// max length of shell command line
#define SHELL_MAX_CLI_LENGTH 64

/// max length of shell output
#define SHELL_MAX_OUT_LENGTH 256

/// max arguments count
#define SHELL_MAX_ARGS 4

/// shell input buffer, received from uart
extern char shell_input_buffer[SHELL_MAX_CLI_LENGTH];

/// shell cli current length
extern uint16_t shell_in_lastchar;

/// shell output buffer, will be sent to uart
extern char shell_output_buffer[SHELL_MAX_OUT_LENGTH];

/// shell output buffer current length
extern uint16_t shell_out_lastchar;

/**
 * @brief add string to output buffer
 * @param s[] string which content will be added to {@link #shell_output_buffer}
 */
void shell_out_buffer_add(const char s[]);

/**
 * @brief add char to {@link #shell_input_buffer}
 * @param c - received character
 * @return boolean - non-true on overflow
 */
boolean shell_in_buffer_add(char c);

/**
 * @brief clean shell output buffer
 *
 * clean {@link #shell_output_buffer} for later use
 */
void shell_cleanup_output(void);

/**
 * @brief shell cli processing
 * see in {@link #shell_input_buffer} and run corresponding commands
 * from {@link #cmds} with parameters
 */
void shell_process(void);

/**
 * @brief send content of {@link #shell_output_buffer} to uart
 * will send to uart {@link #shell_output_buffer} and clean
 * {@link #shell_input_buffer} and {@link #shell_output_buffer}
 */
void shell_send_result(void);

/**
 * @brief shell processing rtos task
 *
 * @addtogroup rtos
 * @{
 */
void shell_task(void *args __attribute((unused)));
///@}

#endif // ifdef SHELL_H_
///@}
