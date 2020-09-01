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
 * all test functions see in tests.h
 */

#include <stdio.h>
#include <string.h>
#include "tests.h"

int main(int argc, char *argv[])
{
    printf("\n********************************\n");
    printf("*       Begin tests...         *\n");
    printf("********************************\n");
    int i = 0;
    while (test_list[i].test_proc != NULL)
    {
        printf("%3d %s\n", i+1, test_list[i].test_str);
        test_list[i].test_proc();
        i++;
    }
    printf("********************************\n");
    printf("*       All tests passed!      *\n");
    printf("********************************\n\n");
}


/** @}*/
