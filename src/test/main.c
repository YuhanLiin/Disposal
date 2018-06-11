#include <stdbool.h>
#include <stdio.h>

#include "file.h"
#include "gpio.h"
#include "config_pin.h"

#include "test/utils.h"

tst_begin_test( file )
{
    char buffer[10];
    size_t len = sizeof( buffer );

    tst_assert_eq_int( write_file( "", "testing" ), false );
    tst_assert_eq_int( read_file( "", buffer, len ), false );
    
    // The file read relies on the file write succeeding
    tst_assert_eq_int( write_file( "test.txt", "testing" ), true );
    tst_assert_eq_int( read_file( "test.txt", buffer, len ), true );
    tst_abort_if_failing();
    tst_assert_eq_str( buffer, "testing" );
} tst_end_test()

tst_begin_test( config_pin )
{
    tst_assert_ne_int( config_pin( 1, 1, PIN_MODE_IN ), 0 );
    
    for( enum Config_pin_mode mode = PIN_MODE_HI; mode < PIN_MODE_MAX; mode++ ){
        tst_assert_eq_int( config_pin( 1, 6, mode ), 0 );
        tst_assert_eq_int( config_pin( 2, 8, mode ), 0 );
    }
} tst_end_test()

tst_begin_test( gpio )
{
    int val;

    tst_assert_eq_int( gpio_write( 6, 0 ), false );
    tst_assert_eq_int( gpio_read( 6, &val ), false );
    
    // Only way right now to set GPIO to output
    tst_assert_eq_int( config_pin( 2, 17, PIN_MODE_OUT ), 0 );
    tst_assert_eq_int( gpio_write( 65, 1 ), true );
    tst_assert_eq_int( gpio_read( 65, &val ), true );
    tst_assert_eq_int( val, 1 );
} tst_end_test()

tst_begin_test_skip( skipped )
{

} tst_end_test()

int setup( void )
{
    printf( "Setup\n" );
    return 0;
}

int teardown( void )
{
    printf( "teardown\n" );
    return 0;
}

int main( void )
{
    tst_assert_ne_str( "good", "stuff" );

    tst_set_setup( setup );
    tst_set_teardown( teardown );
    tst_run_test( file );

    tst_unset_setup();
    tst_unset_teardown();
    tst_run_test( config_pin );

    tst_run_test( gpio );
    tst_run_test( skipped );
    
    tst_report_results();
}

