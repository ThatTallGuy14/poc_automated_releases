#ifndef PIN_DRIVER_HPP
#define PIN_DRIVER_HPP

#include "stm32u0xx_hal.h"
#include "hal-interface/i_generic.hpp"

// STM32 port mapping. Other MCU families can define their own mapping.
enum STMPort : uint8_t {
  STMPortA = 0,
  STMPortB = 1,
  STMPortC = 2,
  STMPortD = 3,
  STMPortE = 4,
  STMPortF = 5,
};

class PinDriver {
public:
  static void EnablePortClock(uint8_t port);
  static GPIO_TypeDef* GetGPIOPort(uint8_t port);
  static uint16_t GetPinNumber(PinID pin);
};

#endif // PIN_DRIVER_HPP
