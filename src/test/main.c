#include <stdbool.h>
#include <stdio.h>

#include "file.h"
#include "gpio.h"
#include "config_pin.h"

#include "test/utils.h"

START_TEST( file )
{
    char buffer[10];
    size_t len = sizeof( buffer );

    INT_ASSERT( write_file( "", "testing" ), false );
    INT_ASSERT( read_file( "", buffer, len ), false );
    
    // The file read relies on the file write succeeding
    INT_ASSERT( write_file( "test.txt", "testing" ), true );
    INT_ASSERT( read_file( "test.txt", buffer, len ), true );
    SKIP_IF_FAILED {
        STR_ASSERT( buffer, "testing" );
    }
} END_TEST()

START_TEST( config_pin )
{
    INT_ASSERT_NE( config_pin( 1, 1, PIN_MODE_IN ), 0 );
    
    for( enum Config_pin_mode mode = PIN_MODE_HI; mode < PIN_MODE_MAX; mode++ ){
        INT_ASSERT( config_pin( 1, 6, mode ), 0 );
        INT_ASSERT( config_pin( 2, 8, mode ), 0 );
    }
} END_TEST()

START_TEST( gpio )
{
    int val;

    INT_ASSERT( gpio_write( 6, 0 ), false );
    INT_ASSERT( gpio_read( 6, &val ), false );
    
    // Only way right now to set GPIO to output
    config_pin( 2, 17, PIN_MODE_OUT );
    INT_ASSERT( gpio_write( 65, 1 ), true );
    INT_ASSERT( gpio_read( 65, &val ), true );
    INT_ASSERT( val, 1 );
} END_TEST()

START_TEST( all )
{
    DEPEND_TEST( file );
    DEPEND_TEST( config_pin );
    DEPEND_TEST( gpio );
} END_TEST()

int main( void )
{
    return RUN_TEST( all );
}
