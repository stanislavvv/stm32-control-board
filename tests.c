/* copyright https://github.com/stanislavvv/stm32-control-board */

/* frameworkless tests... */

#include <stdio.h>
#include <string.h>
#include "tests.h"


int main(int argc, char *argv[])
{
    int i = 0;
    while (test_list[i].test_proc != NULL)
    {
        printf("%d %s\n", i+1, test_list[i].test_str);
        test_list[i].test_proc();
        i++;
    }
    printf("All tests passed\n\n");
}
