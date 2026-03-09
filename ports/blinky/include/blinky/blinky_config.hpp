#ifndef BLINKY_CONFIG_HPP
#define BLINKY_CONFIG_HPP

#include "hal-interface/i_generic.hpp"
#include "hal-interface/i_gpio.hpp"

/**
 * @brief Configuration structure for the Blinky feature
 * 
 * Defines the pins and behavior for the LED and button.
 */
struct BlinkyConfig {
    PinID ledPin;                           ///< LED pin identifier
    PinID buttonPin;                        ///< Button pin identifier
    GPIOPullMode buttonPull;                ///< Button pull-up/down configuration
    GPIOInterruptTrigger buttonTrigger;     ///< Button interrupt trigger mode
};


#endif // BLINKY_CONFIG_HPP
