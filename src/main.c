#include <stdio.h>
#include <stdbool.h>

#include "config_pin.h"
#include "gpio.h"

int main( void )
{
    config_pin( 1, 2, PIN_MODE_IN );
    config_pin( 1, 4, PIN_MODE_OUT );
    config_pin( 1, 6, PIN_MODE_HI );
    config_pin( 1, 8, PIN_MODE_LO );

    gpio_write( 2, true );
    
    int val;
    gpio_read( 2, &val );
    printf( "Read GPIO2: %d\n", val );
    return 0;
}
