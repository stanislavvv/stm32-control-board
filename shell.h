/** @weakgroup shell
 *  @{
 */
/**
 * @file shell.h
 * @brief rtos shell task
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */
/* copyright https://github.com/stanislavvv/stm32-control-board */
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief shell processing rtos task
 */
void task_process_shell(void *args __attribute((unused)));

/**
 * @brief send content of {@link #shell_output_buffer} to uart
 * will send to uart {@link #shell_output_buffer} and clean
 * {@link #shell_input_buffer} and {@link #shell_output_buffer}
 */
void shell_send_result(void);

/** @}*/
