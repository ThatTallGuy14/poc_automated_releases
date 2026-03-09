#ifndef PIN_DRIVER_HPP
#define PIN_DRIVER_HPP

#include "stm32u0xx_hal.h"
#include "hal-interface/i_generic.hpp"

class PinDriver {
public:
  static void EnablePortClock(uint8_t port);
  static GPIO_TypeDef* GetGPIOPort(uint8_t port);
  static uint16_t GetPinNumber(PinID pin);
};

#endif // PIN_DRIVER_HPP
