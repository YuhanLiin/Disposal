#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CONFIG_PIN_FMT "config-pin P%d_%d %s"
#define MAX_CONFIG_PIN_CMD_LEN 30

enum Config_pin_mode {
    PIN_GPIO_HI,
    PIN_GPIO_LO,
    PIN_GPIO_IN,
    PIN_GPIO_OUT
};

int config_pin( int header_num, int pin_num, enum Config_pin_mode mode )
{
    assert( header_num == 1 || header_num == 2 );
    assert( pin_num >= 1 && pin_num <= 36 );

    const char * mode_str;
    char cmd[ MAX_CONFIG_PIN_CMD_LEN ];

    switch( mode ){
        case PIN_GPIO_IN:
            mode_str = "in";
            break;
        case PIN_GPIO_OUT:
            mode_str = "out";
            break;
        case PIN_GPIO_LO:
            mode_str = "lo";
            break;
        case PIN_GPIO_HI:
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

#define GPIO_PATH_FMT "/sys/class/gpio/gpio%d/%s"
#define MAX_GPIO_PATH_LEN 50

#define open_file_checked( file, path, mode ) do{\
    file = fopen( path, mode );\
    if( file == NULL ){\
        fprintf( stderr, "Cannot open file %s\n", path );\
        return false;\
    }\
} while( 0 )

// Attempts to write a file and prints error message if it fails.
bool write_file( const char * path, const char * message )
{
    FILE * file;
    open_file_checked( file, path, "w" );
    bool success = true;
    if( fprintf( file, message ) < 0 ){
        fprintf( stderr, "Cannot write to file %s\n", path );
        success = false;
    }
    fclose( file );
    return success;
}

bool read_file( const char * path, char * buffer, size_t buflen )
{
    assert( buffer );

    FILE * file;
    open_file_checked( file, path, "r" );
    bool success = true;
    if( !fgets( buffer, buflen, file ) ){
        fprintf( stderr, "Cannot read from file %s\n", path );
        success = false;
    }
    fclose( file );
    return success;
}

bool gpio_write( int gpio_num, bool on_or_off )
{
    assert( gpio_num >= 2 && gpio_num <= 117 );

    char gpio_path[ MAX_GPIO_PATH_LEN ];
    snprintf( gpio_path, MAX_GPIO_PATH_LEN, GPIO_PATH_FMT, gpio_num, "value" );
    
    if( on_or_off ) return write_file( gpio_path, "1" );
    else return write_file( gpio_path, "0" );
}

bool gpio_read( int gpio_num, bool * on_off_ptr )
{
    assert( on_off_ptr );

    char gpio_path[ MAX_GPIO_PATH_LEN ];
    snprintf( gpio_path, MAX_GPIO_PATH_LEN, GPIO_PATH_FMT, gpio_num, "value" );

    char value_str[ 2 ];
    if ( !read_file( gpio_path, value_str, sizeof( value_str ) ) ){
        return false;
    }
    *on_off_ptr = value_str[ 0 ] - '0';
    return true;
}

int main( void )
{
    config_pin( 1, 2, PIN_GPIO_IN );
    config_pin( 1, 4, PIN_GPIO_OUT );
    config_pin( 1, 6, PIN_GPIO_HI );
    config_pin( 1, 8, PIN_GPIO_LO );

    gpio_write( 2, true );
    
    bool val;
    gpio_read( 2, &val );
    printf( "Read GPIO2: %d\n", val );
    return 0;
}
