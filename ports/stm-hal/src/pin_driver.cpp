#include "stm-hal/pin_driver.hpp"
#include "stm32u0xx_hal.h"

// Get the STM32 GPIO port base address from port number
GPIO_TypeDef* PinDriver::GetGPIOPort(uint8_t port) {
  switch(port) {
    case 0: return GPIOA;
    case 1: return GPIOB;
    case 2: return GPIOC;
    case 3: return GPIOD;
    default: return GPIOA; // Default to GPIOA
  }
}

// Get the STM32 GPIO pin number (as bit position)
uint16_t PinDriver::GetPinNumber(PinID pin) {
  return (uint16_t)(1 << pin.pin);
}

// Enable clock for the GPIO port
void PinDriver::EnablePortClock(uint8_t port) {
  switch(port) {
    case 0: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
    case 1: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
    case 2: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
    case 3: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
    default: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
  }
}
