#ifndef UNITTEST_H
#define UNITTEST_H

#include <stdio.h>

typedef void (*test_func_t)(void);

extern test_func_t test_registry[100];
extern const char* test_names[100];
extern int test_count;
extern int failed_tests;
extern int passed_tests;

#define TEST(name) \
    void test_func_##name(void); \
    __attribute__((constructor)) void register_test_##name(void) { \
        test_registry[test_count] = test_func_##name; \
        test_names[test_count] = #name; \
        test_count++; \
    } \
    void test_func_##name(void)

#define EXPECT_EQ(expected, actual) do  { \
        if((expected) == (actual)) { \
            printf("\t\e[32m[PASS]\e[0m %s:%d\n", __FILE__, __LINE__); \
            passed_tests++; \
        } else { \
            printf("\t\e[31m[FAIL]\e[0m %s:%d\n", __FILE__, __LINE__); \
            printf("\t\texpected: %s actual: %s\n", #expected, #actual); \
            failed_tests++; \
        } \
    } while(0)

int run_all_tests(void);

#ifdef UNITTEST_IMPLEMENTATION
test_func_t test_registry[100];
const char* test_names[100];
int test_count = 0;
int failed_tests = 0;
int passed_tests = 0;

int run_all_tests(void) {
    printf("RUNNING %d TESTS\n\n", test_count);
    for (int i = 0; i < test_count; i++) {
        printf("Running: %s\n", test_names[i]);
        test_registry[i]();
    }
    printf("\nENDED WITH \e[32m%d\e[0m TESTS PASSED AND \e[31m%d\e[0m TESTS FAILED\n", passed_tests, failed_tests);
    return failed_tests;
}
#endif
#endif
