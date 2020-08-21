/* frameworkless tests... */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "shell_process.h"

void test_shell_process(void)
{
    char cmd[32] = "aaa bbb";
    char out[32] = "\0";
    shell_process(out,cmd);
    assert(!strcmp("Processed: aaa",out));
}

