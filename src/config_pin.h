#pragma once

enum Config_pin_mode {
    PIN_MODE_HI,
    PIN_MODE_LO,
    PIN_MODE_IN,
    PIN_MODE_OUT,
    PIN_MODE_MAX    // Unused
};

// Calls config-pin on one of the beagle's pins. Returns exit code.
int config_pin( int header_num, int pin_num, enum Config_pin_mode mode );

