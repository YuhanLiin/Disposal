#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define OPEN_FILE_CHECKED( var_file, path, mode, on_fail ) do {\
    var_file = fopen( path, mode );\
    if( var_file == NULL ){\
        fprintf( stderr, "Cannot open file %s in \"%s\"\n", path, mode );\
        on_fail;\
    }\
} while( 0 )

#define CLOSE_FILE_CHECKED( file, path, on_fail ) do {\
    if( fclose( file ) != 0 ){\
        fprintf( stderr, "Cannot close file %s\n", path );\
        on_fail;\
    }\
} while( 0 )

// Attempts to write a file and prints error message if it fails.
bool write_file( const char * path, const char * message )
{
    FILE * file;
    OPEN_FILE_CHECKED( file, path, "w", return false );

    bool success = true;
    if( fputs( message, file ) < 0 ){
        fprintf( stderr, "Cannot write to file %s\n", path );
        success = false;
    }
    CLOSE_FILE_CHECKED( file, path, success = false );
    return success;
}

bool read_file( const char * path, char * buffer, size_t buflen )
{
    assert( buffer );

    FILE * file;
    OPEN_FILE_CHECKED( file, path, "r", return false );

    bool success = true;
    if( !fgets( buffer, buflen, file ) ){
        fprintf( stderr, "Cannot read from file %s\n", path );
        success = false;
    }
    CLOSE_FILE_CHECKED( file, path, success = false );
    return success;
}
