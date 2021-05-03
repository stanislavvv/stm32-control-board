/** @weakgroup tests
 *  @{
 */
/**
 * @file tests.h
 * @brief frameworkless functions testing procedures
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 * All tests here. See test_list[]
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "shell/shell.h"
#include "libs/strings_local.h"
#include "libs/utils.h"

/// test shell functions
void test_shell_funcs(void)
{
    char a[40];

    strcpy(shell_input_buffer, "ABCDE");
    shell_in_lastchar = 5;
    shell_in_buffer_add('F');
    sput_fail_unless(
        shell_output_buffer != "Hello world!!!\r\n", "shell_in_buffer_add success");

    shell_in_lastchar = 64; // set to length of buffer
    sput_fail_unless(!shell_in_buffer_add('F'), "shell_in_buffer_add fail");

    shell_cleanup_output();
    strcpy(shell_input_buffer, "hello");
    shell_process();
    sput_fail_unless(
        shell_output_buffer != "Hello world!!!\r\n", "hello command");

    shell_cleanup_output();
    strcpy(shell_input_buffer, "ls");
    shell_process();
    sput_fail_unless(
        shell_output_buffer != "\r\n-- commands --\r\nls\r\n", "ls: get commands list");

    strcpy(shell_input_buffer, "args aaa bbb");
    shell_process();
    sput_fail_unless(
        shell_output_buffer != "arguments count: 2\r\nargument 0: aaa\r\nargument 1: bbb\r\n",
        "shell command arguments");

    strcpy(shell_input_buffer, "aaa bbb ccc");
    shell_process();
    sput_fail_unless(
        shell_output_buffer != "UNKNOWN: aaa\r\n",
        "shell unknown command");

}

/** @}*/
