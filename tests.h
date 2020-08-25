/* copyright https://github.com/stanislavvv/stm32-control-board
 *
 * frameworkless tests...
 * all tests here
 *
 * test_list[] - list of test functions with short description
 *
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "shell_process.h"

void test_shell_process_args(void)
{
    // prepare data
    strcpy(shell_input_buffer, "args aaa bbb");
    shell_process();
    //test
    assert(!strcmp("arguments count: 2\r\nargument 0: aaa\r\nargument 1: bbb\r\n", shell_output_buffer));
}

void test_shell_process_unknown(void)
{
    // prepare data
    strcpy(shell_input_buffer, "aaa bbb");
    shell_process();
    // test
    assert(!strcmp("UNKNOWN: aaa\r\n", shell_output_buffer));
}

void test_shell_process_hello(void)
{
    // prepare data
    strcpy(shell_input_buffer, "hello");
    shell_process();
    // test
    assert(!strcmp("Hello world!!!\r\n", shell_output_buffer));
}

void test_shell_buffer_add(void)
{
    char c = 'A';
    assert(!strcmp(shell_input_buffer, ""));
    shell_in_buffer_add(c);
    assert(!strcmp(shell_input_buffer, "A"));
}

typedef void (*test_handler_t)(void);

typedef struct // test name + test function
{
    const char* test_str;
    test_handler_t test_proc;
} test_def_t;

/* list of running tests */
static test_def_t test_list[] =
{
    {"shell_buffer_add", test_shell_buffer_add},
    {"shell_process_hello", test_shell_process_hello},
    {"shell_process_args", test_shell_process_args},
    {"shell_process_unknown", test_shell_process_unknown},
    {NULL, NULL}
};
