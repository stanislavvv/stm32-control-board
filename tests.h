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
    shell_cleanup_output();
    strcpy(shell_input_buffer, "ls");
    shell_process();
    sput_fail_unless(
        shell_output_buffer != "\r\n-- commands --\r\nls\r\n", "ls: get commands list");

    strcpy(shell_input_buffer, "args aaa bbb");
    shell_process();
    sput_fail_unless(
        shell_output_buffer != "arguments count: 2\r\nargument 0: aaa\r\nargument 1: bbb\r\n",
        "shell arguments");
}


#if 0
/// test shell command arguments processing
void test_shell_args_cmd(void)
{
    // prepare data
    strcpy(shell_input_buffer, "args aaa bbb");
    shell_process();
    //test
    assert(!strcmp("arguments count: 2\r\nargument 0: aaa\r\nargument 1: bbb\r\n",
           shell_output_buffer));
}

/// test hello shell command
void test_shell_hello_cmd(void)
{
    // prepare data
    strcpy(shell_input_buffer, "hello");
    shell_process();
    // test
    assert(!strcmp("Hello world!!!\r\n", shell_output_buffer));
}

/// test shell reaction to unknown command
void test_shell_unknown_cmd(void)
{
    // prepare data
    strcpy(shell_input_buffer, "aaa bbb");
    shell_process();
    // test
    assert(!strcmp("UNKNOWN: aaa\r\n", shell_output_buffer));
}

/// test shell_out_buffer_add
void test_shell_out_buffer_add(void)
{
    char c[] = "B";
    assert(!strcmp(shell_output_buffer, ""));
    shell_out_buffer_add(c);
    assert(!strcmp(shell_output_buffer, "B"));
}

/// test shell_in_buffer_add
void test_shell_in_buffer_add(void)
{
    char c = 'A';
    assert(!strcmp(shell_input_buffer, ""));
    shell_in_buffer_add(c);
    assert(!strcmp(shell_input_buffer, "A"));
}

/// test shell_cleanup_output
void test_shell_cleanup_output(void)
{
    char c[] = "ABCDEF";
    strncpy(shell_output_buffer, c, strlen(c));
    shell_cleanup_output();
    assert(strlen(shell_output_buffer)==0);
}

/// test procedure pointer type
typedef void (*test_handler_t)(void);

/// test procedure list entry type
typedef struct
{
    const char* test_str; ///< name of test, usually testing function name
    test_handler_t test_proc; ///< test procedure
    const uint16_t test_group_id; ///< test group id
} test_def_t;

/// test group list entry type
typedef struct
{
    uint16_t group_id; ///< test group id
    const char* group_name; /// test group name
} test_groups_t;

/// list of test groups
static test_groups_t group_list[] =
{
    {1, "string_local.h"},
    {2, "shell functions"},
    {3, "utils"},
    {0, NULL}
};

/// list of tests
static test_def_t test_list[] =
{
// template line
//    {"",               test_},
    {"i2bin",                 test_i2bin, 1},
    {"itobin_u32",            test_itobin_u32, 1},
    {"itohex_u32",            test_itohex_u32, 1},
    {"itoa_s16",              test_itoa_s16, 1},
    {"itoa_u16",              test_itoa_u16, 1},
    {"reverse",               test_reverse, 1},
    {"compare_strings",       test_compare_strings, 1},
    {"strnsmp_local",         test_strncmp_local, 1},
    {"strlen_local",          test_strlen_local, 1},
    {"shell_in_buffer_add",   test_shell_in_buffer_add, 2},
    {"shell_out_buffer_add",  test_shell_out_buffer_add, 2},
    {"shell_cleanup_output",  test_shell_cleanup_output, 2},
    {"shell_unknown_cmd",     test_shell_unknown_cmd, 2},
    {"shell_hello_cmd",       test_shell_hello_cmd, 2},
    {"shell_args_cmd",        test_shell_args_cmd, 2},
    {"shell_ls_cmd",          test_shell_ls_cmd, 2},
    {NULL, NULL, 0}
};

#endif // if 0

/** @}*/
