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
#include "strings_local.h"
#include "utils.h"

/** test reverse_bits */
void test_reverse_bits(void)
{
    uint16_t a = 0xd8;
    uint16_t b = reverse_bits(a, 8);
    uint16_t c = reverse_bits(a, 16);
    assert(b == 0x1b);
    assert(c == 0x1b00);
}

/** test shell_cmds */
void test_shell_cmds(void)
{
    shell_cleanup_output();
    shell_cmds(NULL, 0);
    char a[] = "\r\n-- commands --\r\nhello\r\n";
    assert(0 == strncmp(a, shell_output_buffer, strlen(a)));
}

/** test i2bin */
void test_i2bin(void)
{
    uint32_t n = 0x13d800ac;
    char a[40];
    char b[] = "0001 0011 1101 1000 0000 0000 1010 1100";
    i2bin(n, a, 8);
    assert(!strcmp(a, b));

}

/** test i2bin */
void test_itobin_u32(void)
{
    uint32_t n = 0x13d800ac;
    char a[40];
    char b[] = "10011110110000000000010101100";
    itobin_u32(n, a);
    assert(!strcmp(a, b));

}

/** test itohex_u32 */
void test_itohex_u32(void)
{
    uint32_t n = 508000172;
    char a[10];
    char b[10];
    itohex_u32(n, a);
    sprintf(b, "%x", n);
    assert(!strcmp(a, b));
}

/** test itoa_s16 */
void test_itoa_s16(void)
{
    uint16_t n = -12345;
    char a[10];
    itoa_s16(n, a);
    assert(!strcmp("-12345", a));
}

/** test itoa_u16 */
void test_itoa_u16(void)
{
    uint16_t n = 12345;
    char a[10];
    itoa_u16(n, a);
    assert(!strcmp("12345", a));
}

/** test reverse */
void test_reverse(void)
{
    char a[] = "ABCDEF";
    reverse(a);
    assert(!strcmp("FEDCBA", a));
}

/** test compare_strings */
void test_compare_strings(void)
{
    const char a1[] = "ABCDE";
    const char a2[] = "ABCDE";
    const char a3[] = "ABCDEF";
    assert(compare_strings(a1, a2));
    assert(!compare_strings(a1, a3));
}

/** test strncmp_local */
void test_strncmp_local(void)
{
    const char a1[] = "ABCDE";
    const char a2[] = "ABCDEF";
    // strncmp_local return TRUE on compare
    assert(strncmp(a1, a2, 4) == !strncmp_local(a1, a2, 4));
    assert(!(strncmp(a1, a2, 6) == !strncmp_local(a1, a2, 6)));
}

/** test strlen_local */
void test_strlen_local(void)
{
    const char a[] = "ABCDE\0";
    assert(strlen(a) == strlen_local(a));
}

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
    const uint16_t test_group_id;
} test_def_t;

typedef struct // group id + group name
{
    uint16_t group_id;
    const char* group_name;
} test_groups_t;

static test_groups_t group_list[] =
{
    {1, "string_local.h"},
    {2, "shell functions"},
    {3, "utils"},
    {0, NULL}
};

/**
 * list of running tests
 */
static test_def_t test_list[] =
{
//    {"",               test_},
    {"reverse_bits",          test_reverse_bits, 3},
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
    {"shell_process_hello",   test_shell_process_hello, 2},
    {"shell_process_args",    test_shell_process_args, 2},
    {"shell_cmds",            test_shell_cmds, 2},
    {"shell_process_unknown", test_shell_process_unknown, 2},
    {NULL, NULL, 0}
};

/** @}*/
