#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "config_pin.h"

#define CONFIG_PIN_FMT "config-pin P%d_%d %s"
#define MAX_CONFIG_PIN_CMD_LEN 30

int config_pin( int header_num, int pin_num, enum Config_pin_mode mode )
{
    assert( header_num == 1 || header_num == 2 );
    // Pin numbers must be in this range, though not all numbers in the range are valid
    assert( pin_num >= 1 && pin_num <= 36 );

    const char * mode_str;
    char cmd[ MAX_CONFIG_PIN_CMD_LEN ];

    switch( mode ){
        case PIN_MODE_IN:
            mode_str = "in";
            break;
        case PIN_MODE_OUT:
            mode_str = "out";
            break;
        case PIN_MODE_LO:
            mode_str = "lo";
            break;
        case PIN_MODE_HI:
            mode_str = "hi";
            break;
        default:
            assert( 0 && "Invalid config-pin mode" );
    }

    snprintf( cmd, MAX_CONFIG_PIN_CMD_LEN, CONFIG_PIN_FMT, header_num, pin_num, mode_str );
    int ret = system( cmd );
    if ( ret != 0 ){
        fprintf( stderr, "config-pin failed with code %d\n", ret );
    }
    return ret;
}
