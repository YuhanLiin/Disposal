#pragma once

#include <stdio.h>
#include <string.h>

#define HASH_SIGNS " #################### "

// Generates name of test function. Name parameter must be unique for all created tests
#define TEST( name ) _func_test_##name
#define RUN_TEST( name ) TEST( name )()

// Runs another test inside a test function
#define DEPEND_TEST( name ) _pass_status = RUN_TEST( name )

// Mark tests as skippable if previous tests have failed to avoid nasty UB
#define SKIP_IF_FAILED if( _pass_status != 0 )

// Initializes a new test function. Must be matched by an END_TEST.
#define START_TEST( name ) int TEST( name )(void)\
{\
    int _pass_status = 0;\
    const char * _test_name = #name;

// Returns pass status of test and prints pass/fail message in stdin
#define END_TEST()\
    if( _pass_status ){\
        printf( "\n"HASH_SIGNS"Test suite \"%s\" FAILED."HASH_SIGNS"\n", _test_name );\
    } else {\
        printf( "\n"HASH_SIGNS"Test suite \"%s\" PASSED."HASH_SIGNS"\n", _test_name );\
    }\
    return _pass_status;\
}

// Asserts equality. Type and format string of the values are needed
// Requires comparator macro that returns true if the two values trigger a test failure 
#define ASSERT( expr, expected, fmt_spec, type, cmp ) do {\
    type _expr = expr;\
    type _expected = expected;\
    if( cmp( _expr, _expected ) ){\
        fprintf(\
            stderr,\
            "Assert error in %s line %d, in suite %s: ",\
            __FILE__, __LINE__, _test_name\
        );\
        fprintf(\
            stderr,\
            "Expected %s to be "fmt_spec" but got "fmt_spec" instead.\n",\
            #expr, _expected, _expr\
        );\
        _pass_status = 1;\
    }\
} while( 0 )

// Type-specific asserts
#define INT_CMP( a, b ) (a != b)
#define INT_ASSERT( expr, expected ) ASSERT( expr, expected, "%d", int, INT_CMP )
#define INT_ASSERT_NE( expr, expected ) ASSERT( expr, expected, "%d", int, !INT_CMP )

#define STR_CMP( a, b ) (strcmp( a, b ) != 0)
#define STR_ASSERT( expr, expected ) ASSERT( expr, expected, "\"%s\"", const char *, STR_CMP )
#define STR_ASSERT_NE( expr, expected ) ASSERT( expr, expected, "\"%s\"", const char *, !STR_CMP )

