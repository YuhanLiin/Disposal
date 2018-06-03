#pragma once

enum Config_pin_mode {
    PIN_GPIO_HI,
    PIN_GPIO_LO,
    PIN_GPIO_IN,
    PIN_GPIO_OUT
};

int config_pin( int header_num, int pin_num, enum Config_pin_mode mode );

