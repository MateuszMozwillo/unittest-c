#ifndef UNITTEST_H
#define UNITTEST_H

#ifndef UT_MAX_TEST_COUNT
#define UT_MAX_TEST_COUNT 1000
#endif

#ifndef REAL_NUMER_EQUAL_MARGIN
#define REAL_NUMER_EQUAL_MARGIN 0.00001
#endif

#ifndef UT_ONLY_SHOW_FAILED
#define UT_ONLY_SHOW_FAILED 0
#endif

#include <stdio.h>

typedef void (*test_func_t)(void);

extern test_func_t unittest_test_registry[UT_MAX_TEST_COUNT];
extern const char* unittest_test_names[UT_MAX_TEST_COUNT];
extern int unittest_test_count;
extern int unittest_failed_tests;
extern int unittest_passed_tests;

#define TEST(name) \
    void unittest_test_func_##name(void); \
    __attribute__((constructor)) void unittest_register_test_##name(void) { \
    unittest_test_registry[unittest_test_count] = unittest_test_func_##name; \
    unittest_test_names[unittest_test_count] = #name; \
    unittest_test_count++; \
    } \
    void unittest_test_func_##name(void)

#define EXPECT_EQ(expected, actual) do  { \
    __typeof__(expected) _ut_exp = (expected); \
    __typeof__(actual) _ut_act = (actual); \
    if(_ut_exp == _ut_act) { \
        if (!UT_ONLY_SHOW_FAILED) { \
            printf("\t\x1b[32m[PASS]\x1b[0m %s : %d\n", __FILE__, __LINE__); \
        } \
        unittest_passed_tests++; \
    } else { \
        printf("\t\x1b[31m[FAIL]\x1b[0m %s : %d\n", __FILE__, __LINE__); \
        printf("\t\texpected: %d got %d\n", _ut_exp, _ut_act); \
        unittest_failed_tests++; \
    } \
} while(0)

int run_all_tests(void);

#ifdef UNITTEST_IMPLEMENTATION
test_func_t unittest_test_registry[UT_MAX_TEST_COUNT];
const char* unittest_test_names[UT_MAX_TEST_COUNT];
int unittest_test_count = 0;
int unittest_failed_tests = 0;
int unittest_passed_tests = 0;

int run_all_tests(void) {
    printf("RUNNING %d TESTS\n", unittest_test_count);
    for (int i = 0; i < unittest_test_count; i++) {
        int _ut_failed_start = unittest_failed_tests;
        printf("\nRunning test group: \x1b[36m%s\x1b[0m\n", unittest_test_names[i]);
        unittest_test_registry[i]();
        if (unittest_failed_tests == _ut_failed_start) {
            printf("\x1b[32m[PASSED]\x1b[0m\n");
        } else {
            printf("\x1b[31m[FAILED]\x1b[0m with \x1b[31m%d\x1b[0m tests\n", unittest_failed_tests - _ut_failed_start);
        }

    }
    printf("\nENDED WITH \e[32m%d\e[0m TESTS PASSED AND \e[31m%d\e[0m TESTS FAILED\n", unittest_passed_tests, unittest_failed_tests);
    return unittest_failed_tests;
}
#endif
#endif
