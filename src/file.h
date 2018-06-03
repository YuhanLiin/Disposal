#pragma once

#include <stdbool.h>

bool read_file( const char * path, char * buffer, size_t buflen );
bool write_file( const char * path, const char * message );

