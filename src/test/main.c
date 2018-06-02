#include <stdbool.h>
#include <stdio.h>

#include "file.h"
#include "gpio.h"
#include "config_pin.h"

#include "test/utils.h"

START_TEST( all ){
    INT_ASSERT( 1, 1 );
} END_TEST()

int main( void )
{
    RUN_TEST( all );
    return 0;

}
