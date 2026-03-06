#ifndef GPIO_DRIVER_HPP
#define GPIO_DRIVER_HPP

#include "stm32u0xx_hal.h"
#include "hal_interface/i_gpio.hpp"
#include <map>

// Forward declarations
class PinDriver;

class GPIODriver : public I_GPIODriver {
public:
  GPIODriver() {}
  virtual ~GPIODriver() {}

  // Initialize the GPIO driver
  virtual void Init() override;

  // Configure a pin with full configuration
  virtual void ConfigurePin(const GPIOConfig& config) override;

  // Set pin state (for output pins)
  virtual void SetPin(PinID pin, GPIOPinState state) override;

  // Read pin state (for input pins)
  virtual GPIOPinState ReadPin(PinID pin) override;

  // Toggle pin state
  virtual void TogglePin(PinID pin) override;

  // Enable/disable interrupt for a pin
  virtual void EnableInterrupt(PinID pin, bool enable) override;

private:
  IRQn_Type GetIRQn(uint8_t pinNumber);
};

#endif // GPIO_DRIVER_HPP
