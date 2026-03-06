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

// Default pin assignments - can be overridden by defining before including this file

#ifndef LED_PIN_PORT
#define LED_PIN_PORT 0  // GPIOA
#endif

#ifndef LED_PIN_NUMBER
#define LED_PIN_NUMBER 5  // PA5
#endif

#ifndef BUTTON_PIN_PORT
#define BUTTON_PIN_PORT 2  // GPIOC
#endif

#ifndef BUTTON_PIN_NUMBER
#define BUTTON_PIN_NUMBER 13  // PC13 (typical user button)
#endif

// Default configuration instance
constexpr BlinkyConfig DEFAULT_BLINKY_CONFIG = {
    .ledPin = {LED_PIN_PORT, LED_PIN_NUMBER},
    .buttonPin = {BUTTON_PIN_PORT, BUTTON_PIN_NUMBER},
    .buttonPull = GPIOPullMode::PullUp,
    .buttonTrigger = GPIOInterruptTrigger::FallingEdge
};

#endif // BLINKY_CONFIG_HPP
