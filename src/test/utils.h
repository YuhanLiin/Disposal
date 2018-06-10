#pragma once

#include <stdio.h>
#include <string.h>

struct _tst_assertion_stats_t {
    unsigned passed;
    unsigned failed;
};

struct _tst_test_stats_t {
    unsigned passed;
    unsigned failed;
    unsigned skipped;
};

// Global test stat trackers used for final reporting
static struct _tst_assertion_stats_t _tst_assertion_stats = {0};
static struct _tst_test_stats_t _tst_test_stats = {0};
// The pointer is guaranteed to point to global data, since the struct variable may be shadowed
static struct _tst_assertion_stats_t * _g_tst_assertion_stats_ptr = &_tst_assertion_stats;

// Symbols
#define _tst_checkmark "\u2713"
#define _tst_crossmark "\u2717"
#define _tst_circle "\u25EF"
#define _tst_checkmark_line _tst_checkmark _tst_checkmark _tst_checkmark _tst_checkmark
#define _tst_crossmark_line _tst_crossmark _tst_crossmark _tst_crossmark _tst_crossmark

// Generates name of test functions. Used to generate function definitions and calls
#define _tst_gen_name( name ) _tst_func_##name

// Mark tests as skippable if previous tests have failed to avoid nasty UB
#define SKIP_IF_FAILED if( _pass_status != 0 )

// Returns whether the test case or the suite is passing based on asserts.
#define tst_passing() (_tst_assertion_stats.failed == 0)

// Convenient if wrappers for determining test/fail
#define tst_if_pass() if( tst_passing() )
#define tst_if_fail() if( !tst_passing() )

#define _tst_report_asserts()\
    printf(\
        "- Assertions: %u attempted, %u passed, %u failed\n",\
        _tst_assertion_stats.passed + _tst_assertion_stats.failed,\
        _tst_assertion_stats.passed, _tst_assertion_stats.failed\
    );\

// Only report tests if tests have actually been made and run
#define _tst_report_tests() do {\
    unsigned _tst_total_tests =\
        _tst_test_stats.passed + _tst_test_stats.failed + _tst_test_stats.skipped;\
    if( _tst_total_tests > 0 ) printf(\
        "- Test cases: %u attemped, %u passed, %u failed, %u skipped\n",\
        _tst_total_tests,\
        _tst_test_stats.passed, _tst_test_stats.failed, _tst_test_stats.skipped\
    );\
} while( 0 )

// Final reporting called from main to print all test results and return success code
#define tst_report_results() do {\
    printf( "\n" );\
    if( tst_passing() ){\
        printf( _tst_checkmark_line " Test case %s PASSED! " _tst_checkmark_line "\n", __FILE__ );\
    } else {\
        printf( _tst_crossmark_line " Test case %s FAILED! " _tst_crossmark_line "\n", __FILE__ );\
    }\
    _tst_report_tests();\
    _tst_report_asserts();\
    /* Since we're returning to shell, we return 0 if the tests are passing, 1 otherwise */\
    return !tst_passing();\
} while( 0 )

// Sync the assert stats from within a test case to the global assert stats.
// Should be updated for new assert stats. DON'T USE OUTSIDE TEST CASES
#define _tst_sync() do {\
    _g_tst_assertion_stats_ptr->passed += _tst_assertion_stats.passed;\
    _g_tst_assertion_stats_ptr->failed += _tst_assertion_stats.failed;\
} while( 0 )

// Initializes a new test function. Must be matched by an END_TEST.
// name =  unique identifier name of the test
#define tst_begin_test( name ) void _tst_gen_name( name )( void )\
{\
    static struct _tst_assertion_stats_t _tst_assertion_stats = {0};\
    const char * _tst_name = #name;

// Version of tst_begin_test that skips the whole test case.
#define tst_begin_test_skip( name ) tst_begin_test( name )\
    printf( "\n" _tst_circle " Test case \"%s\" SKIPPED!\n", _tst_name );\
    _tst_test_stats.skipped += 1;\
    return;
    
// Prints result message depending on whether the test case passed or failed
// and reports assert stats for this test
#define tst_end_test()\
    if( tst_passing() ){\
        printf( "\n" _tst_checkmark " Test case \"%s\" PASSED!\n", _tst_name );\
        _tst_test_stats.passed += 1;\
    } else {\
        printf( "\n" _tst_crossmark " Test case \"%s\" FAILED!\n", _tst_name );\
        _tst_test_stats.failed += 1;\
    }\
    _tst_report_asserts();\
    _tst_sync();\
    return;\
}

// Aborts a test, which marks it as failed
#define tst_abort_test() do {\
    printf( "\n" _tst_crossmark " Test case \"%s\" ABORTED!\n", _tst_name );\
    _tst_report_asserts();\
    _tst_test_stats.failed += 1;\
    _tst_sync();\
    return;\
} while( 0 )

// Abort test when failing
 #define tst_abort_if_failing() tst_if_fail() tst_abort_test()

// Runs a test runner function of the specified name
#define tst_run_test( name ) _tst_gen_name( name )()

// Asserts equality. Type and format string of the values are needed
// Requires comparator macro that returns true if the two values result in passing the test
#define _tst_assert_base( expr, expected, fmt_spec, type, cmp ) do {\
    type _expr = expr;\
    type _expected = expected;\
    if( !cmp( _expr, _expected ) ){\
        fprintf(\
            stderr,\
            "Assert error in %s:%d: ",\
            __FILE__, __LINE__\
        );\
        fprintf(\
            stderr,\
            "Expected %s to be "fmt_spec" but got "fmt_spec" instead.\n",\
            #expr, _expected, _expr\
        );\
        _tst_assertion_stats.failed += 1;\
    } else {\
        _tst_assertion_stats.passed += 1;\
    }\
} while( 0 )

// Int asserts
#define _tst_int_cmp( a, b ) (a == b)

#define tst_assert_int( expr, expected, cmp )\
    _tst_assert_base( expr, expected, "%d", int, cmp )

#define tst_assert_eq_int( expr, expected )\
    tst_assert_int( expr, expected, _tst_int_cmp ) 

#define tst_assert_ne_int( expr, expected )\
    tst_assert_int( expr, expected, !_tst_int_cmp ) 

// String-type asserts
#define _tst_str_cmp( a, b ) (strcmp( a, b ) == 0)

#define tst_assert_str( expr, expected, cmp )\
    _tst_assert_base( expr, expected, "\"%s\"", const char *, cmp )

#define tst_assert_eq_str( expr, expected )\
    tst_assert_str( expr, expected, _tst_str_cmp )

#define tst_assert_ne_str( expr, expected )\
    tst_assert_str( expr, expected, !_tst_str_cmp )

