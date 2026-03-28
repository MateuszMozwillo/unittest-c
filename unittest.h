#ifndef UNITTEST_H
#define UNITTEST_H

#ifndef MAX_TEST_COUNT
#define MAX_TEST_COUNT 1000
#endif

#include <stdio.h>

typedef void (*test_func_t)(void);

extern test_func_t unittest_test_registry[MAX_TEST_COUNT];
extern const char* unittest_test_names[MAX_TEST_COUNT];
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
    __typeof__(expected) _exp = (expected); \
    __typeof__(actual) _act = (actual); \
    if(_exp == _act) { \
        printf("\t\e[32m[PASS]\e[0m %s:%d\n", __FILE__, __LINE__); \
        unittest_passed_tests++; \
    } else { \
        printf("\t\e[31m[FAIL]\e[0m %s:%d\n", __FILE__, __LINE__); \
        printf("\t\texpected: %d actual: %d\n", _exp, _act); \
        unittest_failed_tests++; \
    } \
} while(0)

int run_all_tests(void);

#ifdef UNITTEST_IMPLEMENTATION
test_func_t unittest_test_registry[MAX_TEST_COUNT];
const char* unittest_test_names[MAX_TEST_COUNT];
int unittest_test_count = 0;
int unittest_failed_tests = 0;
int unittest_passed_tests = 0;

int run_all_tests(void) {
    printf("RUNNING %d TESTS\n\n", unittest_test_count);
    for (int i = 0; i < unittest_test_count; i++) {
        printf("Running: %s\n", unittest_test_names[i]);
        unittest_test_registry[i]();
    }
    printf("\nENDED WITH \e[32m%d\e[0m TESTS PASSED AND \e[31m%d\e[0m TESTS FAILED\n", unittest_passed_tests, unittest_failed_tests);
    return unittest_failed_tests;
}
#endif
#endif
