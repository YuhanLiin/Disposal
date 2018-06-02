#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "file.h"

#define GPIO_PATH_FMT "/sys/class/gpio/gpio%d/%s"
#define MAX_GPIO_PATH_LEN 50

bool gpio_write( int gpio_num, int val )
{
    assert( gpio_num >= 2 && gpio_num <= 117 );
    assert( val == 1 || val == 0 );

    char gpio_path[ MAX_GPIO_PATH_LEN ];
    snprintf( gpio_path, MAX_GPIO_PATH_LEN, GPIO_PATH_FMT, gpio_num, "value" );
    
    if( val ) return write_file( gpio_path, "1" );
    else return write_file( gpio_path, "0" );
}

bool gpio_read( int gpio_num, int * val_ptr )
{
    assert( val_ptr );

    char gpio_path[ MAX_GPIO_PATH_LEN ];
    snprintf( gpio_path, MAX_GPIO_PATH_LEN, GPIO_PATH_FMT, gpio_num, "value" );

    char value_str[ 2 ];
    if ( !read_file( gpio_path, value_str, sizeof( value_str ) ) ){
        return false;
    }
    *val_ptr = value_str[ 0 ] - '0';
    return true;
}

