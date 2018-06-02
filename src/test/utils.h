#pragma once

#include <stdio.h>

#define HASH_SIGNS " #################### "

// Generates name of test function. Name parameter must be unique for all created tests
#define TEST( name ) _fun_test_##name
#define RUN_TEST( name ) TEST( name )()

// Initializes a new test function. Must be matched by an END_TEST.
#define START_TEST( name ) int TEST( name )(void)\
{\
    int _pass_status = 0;\
    const char * _test_name = #name;

// Returns pass status of test and prints pass/fail message
#define END_TEST()\
    if( _pass_status ){\
        printf( "\n"HASH_SIGNS"Test suite %s failed."HASH_SIGNS"\n", _test_name );\
    } else {\
        printf( "\n"HASH_SIGNS"Test suite %s passed."HASH_SIGNS"\n", _test_name );\
    }\
    return _pass_status;\
}

// Asserts equality. Type and format string of the values are needed
#define ASSERT( expr, expected, fmt_spec, type ) do {\
    type _expr = expr;\
    type _expected = expected;\
    if( _expr != expected ){\
        fprintf(\
            stderr,\
            "Assert error in %s line %d, in %s: ",\
            __FILE__, __LINE__, __func__\
        );\
        fprintf(\
            stderr,\
            "Expected %s to be "fmt_spec" but got "fmt_spec" instead.\n",\
            #expr, _expected, _expr\
        );\
        _pass_status = 1;\
    }\
} while( 0 )

#define INT_ASSERT( expr, expected ) ASSERT( expr, expected, "%d", int )
