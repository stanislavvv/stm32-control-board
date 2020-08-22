/* frameworkless tests... */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "shell_process.h"

//void test_shell_process(void);

void test_shell_process(void)
{
    char cmd[32] = "aaa bbb";
    char out[32] = "\0";
    shell_process(out,cmd);
    assert(!strcmp("Processed: aaa", out));
}

void test_shell_buffer_add(void)
{
    char c = 'A';
    assert(!strcmp(shell_input_buffer, ""));
    shell_buffer_add(c);
    assert(!strcmp(shell_input_buffer, "A"));
}

typedef void (*test_handler_t)(void);

typedef struct {
    const char* test_str;
    test_handler_t test_proc;
} test_def_t;

/* list of running tests */
static test_def_t test_list[] =
{
    {"shell_buffer_add", test_shell_buffer_add},
    {"shell_process", test_shell_process},
    {NULL, NULL}
};
