#pragma once

#include <stdbool.h>

bool gpio_read( int gpio_num, int * val_ptr );
bool gpio_write( int gpio_num, int val );
