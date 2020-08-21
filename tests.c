/* copyright https://github.com/stanislavvv/stm32-control-board */

/* frameworkless tests... */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "shell_process.h"


int main(int argc, char *argv[])
{
    char cmd[32] = "aaa bbb";
    char out[32] = "\0";
    shell_process(out,cmd);
    assert(!strcmp("Processed: aaa",out));
    printf("All tests passed\n");
}
