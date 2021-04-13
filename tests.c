/** @weakgroup tests
 *  @{
 */
/**
 * @file tests.c
 * @brief frameworkless functions testing main executable
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 * run tests from test_list with some output
 *
 * all test functions see in tests.h
 */

#include <stdio.h>
#include <string.h>
#include "sput.h"
#include "tests.h"

/**
 * @brief main test procedure
 *
 * run test over test groups
 */
int main(int argc, char *argv[])
{
    sput_start_testing();

    sput_enter_suite("shell functions");
    sput_run_test(test_shell_funcs);

    sput_finish_testing();

    return sput_get_return_value();
}


/** @}*/
