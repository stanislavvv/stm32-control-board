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
    int g = 0;
    while (group_list[g].group_id != 0)
    {
        printf("\n*** testgroup: %d. %s ***\n", g+1, group_list[g].group_name);
        int i = 0;
        int n = 1;
        while (test_list[i].test_proc != NULL)
        {
            if (test_list[i].test_group_id == group_list[g].group_id)
            {
                printf("%3d %s\n", n, test_list[i].test_str);
                test_list[i].test_proc();
                n++;
            }
            i++;
        }
        g++;
    }
    printf("\n********************************\n");
    printf("*       All tests passed!      *\n");
    printf("********************************\n\n");
}


/** @}*/
