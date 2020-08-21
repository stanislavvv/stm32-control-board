/* copyright http://www.jera.com/techinfo/jtns/jtn002.html */

#define mu_assert(message, test) do { if (!(test)) { return message; } } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) { return message;  } } while (0)
extern int tests_run;

/****************************************************************************
No, that's not a typo. It's just 3 lines of code (4 here because I wrapped a long line.)

Setting Up A Test Case

A MinUnit test case is just a function that returns 0 (null) if the tests pass.
If the test fails, the function should return a string describing the failing test.
mu_assert is simply a macro that returns a string if the expression passed to it is false.
The mu_runtest macro calls another test case and returns if that test case fails.
That's all there is to it!

Example
The following example runs two tests, one that passes, one that fails.

#include <stdio.h>
#include "minunit.h"

int tests_run = 0;

int foo = 7;
int bar = 4;

static char * test_foo() {
    mu_assert("error, foo != 7", foo == 7);
    return 0;
}

static char * test_bar() {
    mu_assert("error, bar != 5", bar == 5);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_foo);
    mu_run_test(test_bar);
    return 0;
}

int main(int argc, char **argv) {
char *result = all_tests();
if (result != 0) {
    printf("%s\n", result);
}
else {
    printf("ALL TESTS PASSED\n");
}
printf("Tests run: %d\n", tests_run);

return result != 0;
}
***************************************************************************/
