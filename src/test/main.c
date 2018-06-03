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

}

int main( void )
{
    return RUN_TEST( all );
}
