#ifndef MINUNIT_H
#define MINUNIT_H

#include <stdio.h>
#include <stdlib.h>

#define mu_suite_start() char *message = NULL;

#define mu_assert(test, message) \
        if (!(test)) {\
                fprintf(stderr, "%s", message); \
                return message; \
        }

#define mu_run_test(test) \
        fprintf(stderr, "\n----- %s ", #test); \
        message = test(); \
        tests_run++; \
        if (message) { \
                return message; \
        }

#define RUN_TESTS(name) \
        int main (int argc, char *argv[]) { \
                (void)argc; \
                fprintf(stderr, "-----\nRUNNING: %s\n", argv[0]); \
                char *result = name(); \
                if (result != 0) { \
                        fprintf(stderr, "\n\nFAILED: %s\n", result); \
                } else { \
                        fprintf(stderr, "\n\nALL TESTS PASSED\n"); \
                } \
                fprintf(stderr, "Tests run: %d\n", tests_run); \
                exit(result != 0); \
        }

int tests_run;

#endif /* MINUNIT_H */
