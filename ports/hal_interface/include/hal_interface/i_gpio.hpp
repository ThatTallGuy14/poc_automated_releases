#ifndef I_GPIO_H
#define I_GPIO_H

#include <stdint.h>
#include "i_generic.hpp"

enum GPIOPinState { High, Low };

enum GPIOPinMode { Input, Output };

enum GPIOPullMode { NoPull, PullUp, PullDown };

enum GPIOInterruptTrigger { 
  NoInterrupt,
  RisingEdge, 
  FallingEdge, 
  BothEdges 
};

// Callback type for GPIO interrupts
using GPIOInterruptCallback = void (*)(PinID pin, void* userData);

struct GPIOConfig {
  PinID pin;
  GPIOPinMode mode;
  GPIOPullMode pull;
  GPIOInterruptTrigger interruptTrigger;
  GPIOInterruptCallback callback;
  void* callbackUserData;
};

class I_GPIODriver {
public:
  I_GPIODriver() {}
  virtual ~I_GPIODriver() {}
  
  // Initialize the GPIO driver
  virtual void Init() = 0;
  
  // Configure a pin with full configuration
  virtual void ConfigurePin(const GPIOConfig& config) = 0;
  
  // Set pin state (for output pins)
  virtual void SetPin(PinID pin, GPIOPinState state) = 0;
  
  // Read pin state (for input pins)
  virtual GPIOPinState ReadPin(PinID pin) = 0;
  
  // Toggle pin state
  virtual void TogglePin(PinID pin) = 0;
  
  // Enable/disable interrupt for a pin
  virtual void EnableInterrupt(PinID pin, bool enable) = 0;
};

#endif // I_GPIO_H
