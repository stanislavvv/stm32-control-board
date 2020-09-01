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
#include "shell_process.h"

/** test shell command arguments processing */
void test_shell_process_args(void)
{
    // prepare data
    strcpy(shell_input_buffer, "args aaa bbb");
    shell_process();
    //test
    assert(!strcmp("arguments count: 2\r\nargument 0: aaa\r\nargument 1: bbb\r\n",
           shell_output_buffer));
}

/** test shell reaction to unknown command */
void test_shell_process_unknown(void)
{
    // prepare data
    strcpy(shell_input_buffer, "aaa bbb");
    shell_process();
    // test
    assert(!strcmp("UNKNOWN: aaa\r\n", shell_output_buffer));
}

/** test hello shell command */
void test_shell_process_hello(void)
{
    // prepare data
    strcpy(shell_input_buffer, "hello");
    shell_process();
    // test
    assert(!strcmp("Hello world!!!\r\n", shell_output_buffer));
}

/** test shell_out_buffer_add */
void test_shell_out_buffer_add(void)
{
    char c[] = "B";
    assert(!strcmp(shell_output_buffer, ""));
    shell_out_buffer_add(c);
    assert(!strcmp(shell_output_buffer, "B"));
}

/** test shell_cleanup_output */
void test_shell_cleanup_output(void)
{
    char c[] = "ABCDEF";
    strncpy(shell_output_buffer, c, strlen(c));
    assert(!(strlen(shell_output_buffer)==0));
}

/** test shell_in_buffer_add */
void test_shell_in_buffer_add(void)
{
    char c = 'A';
    assert(!strcmp(shell_input_buffer, ""));
    shell_in_buffer_add(c);
    assert(!strcmp(shell_input_buffer, "A"));
}

/**
 * test procedure pointer type
 */
typedef void (*test_handler_t)(void);

/**
 * test procedure list entry type
 */
typedef struct // test name + test function
{
    const char* test_str; /** name of test, usually testing function name */
    test_handler_t test_proc; /** test procedure */
} test_def_t;

/**
 * list of running tests
 */
static test_def_t test_list[] =
{
    {"shell_in_buffer_add",   test_shell_in_buffer_add},
    {"shell_out_buffer_add",  test_shell_out_buffer_add},
    {"shell_cleanup_output",  test_shell_cleanup_output},
    {"shell_process_hello",   test_shell_process_hello},
    {"shell_process_args",    test_shell_process_args},
    {"shell_process_unknown", test_shell_process_unknown},
    {NULL, NULL}
};

/** @}*/
