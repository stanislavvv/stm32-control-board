/* copyright https://github.com/stanislavvv/stm32-control-board
 *
 * frameworkless tests...
 * all test functions see in tests.h
 *
 * main executable
 * run tests from test_list with some output
 *
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
