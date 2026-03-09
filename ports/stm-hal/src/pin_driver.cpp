#include "stm-hal/pin_driver.hpp"
#include "stm32u0xx_hal.h"

// Get the STM32 GPIO port base address from port number
GPIO_TypeDef* PinDriver::GetGPIOPort(uint8_t port) {
  switch(port) {
  case STMPortA: return GPIOA;
  case STMPortB: return GPIOB;
  case STMPortC: return GPIOC;
#ifdef GPIOD
  case STMPortD: return GPIOD;
#endif
#ifdef GPIOE
  case STMPortE: return GPIOE;
#endif
#ifdef GPIOF
  case STMPortF: return GPIOF;
#endif
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
  case STMPortA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
  case STMPortB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
  case STMPortC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
#ifdef __HAL_RCC_GPIOD_CLK_ENABLE
  case STMPortD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
#endif
#ifdef __HAL_RCC_GPIOE_CLK_ENABLE
  case STMPortE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
#endif
#ifdef __HAL_RCC_GPIOF_CLK_ENABLE
  case STMPortF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
#endif
    default: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
  }
}
