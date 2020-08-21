/* copyright https://github.com/stanislavvv/stm32-control-board */

/* frameworkless tests... */

#include <stdio.h>
#include <string.h>
#include "tests.h"


int main(int argc, char *argv[])
{
    printf("testing shell_process\n");
    test_shell_process();
    printf("ok\n");
    printf("All tests passed\n");
}
