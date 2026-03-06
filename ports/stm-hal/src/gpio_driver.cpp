#include "stm-hal/gpio_driver.hpp"
#include "stm-hal/pin_driver.hpp"
#include "stm32u0xx_hal.h"
#include <map>

// COMMENT
// Storage for interrupt callbacks
struct InterruptData {
  GPIOInterruptCallback callback;
  void* userData;
  PinID pin;
};

// Map to store interrupt handlers for each pin
static std::map<uint8_t, InterruptData> g_interruptHandlers;

// Initialize the GPIO driver
void GPIODriver::Init() {
  // Enable SYSCFG clock for EXTI configuration
  __HAL_RCC_SYSCFG_CLK_ENABLE();
}

// Configure a pin with full configuration
void GPIODriver::ConfigurePin(const GPIOConfig& config) {
  // Enable clock for the port
  PinDriver::EnablePortClock(config.pin.port);
  
  GPIO_TypeDef* port = PinDriver::GetGPIOPort(config.pin.port);
  uint16_t pinMask = PinDriver::GetPinNumber(config.pin);
  
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pinMask;
  
  // Set mode
  if (config.mode == GPIOPinMode::Output) {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  } else {
    // Input mode with interrupt setup
    switch(config.interruptTrigger) {
      case GPIOInterruptTrigger::RisingEdge:
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
        break;
      case GPIOInterruptTrigger::FallingEdge:
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
        break;
      case GPIOInterruptTrigger::BothEdges:
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
        break;
      default:
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        break;
    }
  }
  
  // Set pull mode
  switch(config.pull) {
    case GPIOPullMode::PullUp:
      GPIO_InitStruct.Pull = GPIO_PULLUP;
      break;
    case GPIOPullMode::PullDown:
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
      break;
    default:
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      break;
  }
  
  HAL_GPIO_Init(port, &GPIO_InitStruct);
  
  // Store interrupt callback if configured
  if (config.interruptTrigger != GPIOInterruptTrigger::NoInterrupt && config.callback != nullptr) {
    InterruptData data;
    data.callback = config.callback;
    data.userData = config.callbackUserData;
    data.pin = config.pin;
    g_interruptHandlers[config.pin.pin] = data;
    
    // Enable the EXTI interrupt
    IRQn_Type irqn = GetIRQn(config.pin.pin);
    HAL_NVIC_SetPriority(irqn, 2, 0);
    HAL_NVIC_EnableIRQ(irqn);
  }
}

// Set pin state (for output pins)
void GPIODriver::SetPin(PinID pin, GPIOPinState state) {
  GPIO_TypeDef* port = PinDriver::GetGPIOPort(pin.port);
  uint16_t pinMask = PinDriver::GetPinNumber(pin);
  
  GPIO_PinState pinState = (state == GPIOPinState::High) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  HAL_GPIO_WritePin(port, pinMask, pinState);
}

// Read pin state (for input pins)
GPIOPinState GPIODriver::ReadPin(PinID pin) {
  GPIO_TypeDef* port = PinDriver::GetGPIOPort(pin.port);
  uint16_t pinMask = PinDriver::GetPinNumber(pin);
  
  GPIO_PinState state = HAL_GPIO_ReadPin(port, pinMask);
  return (state == GPIO_PIN_SET) ? GPIOPinState::High : GPIOPinState::Low;
}

// Toggle pin state
void GPIODriver::TogglePin(PinID pin) {
  GPIO_TypeDef* port = PinDriver::GetGPIOPort(pin.port);
  uint16_t pinMask = PinDriver::GetPinNumber(pin);
  HAL_GPIO_TogglePin(port, pinMask);
}

// Enable/disable interrupt for a pin
void GPIODriver::EnableInterrupt(PinID pin, bool enable) {
  IRQn_Type irqn = GetIRQn(pin.pin);
  if (enable) {
    HAL_NVIC_EnableIRQ(irqn);
  } else {
    HAL_NVIC_DisableIRQ(irqn);
  }
}

// Get the IRQ number for a pin
IRQn_Type GPIODriver::GetIRQn(uint8_t pinNumber) {
  if (pinNumber <= 1) {
    return EXTI0_1_IRQn;
  } else if (pinNumber <= 3) {
    return EXTI2_3_IRQn;
  } else {
    return EXTI4_15_IRQn;
  }
}

// EXTI interrupt handler helper
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  // Find the pin number from the pin mask
  uint8_t pinNumber = 0;
  uint16_t mask = GPIO_Pin;
  while (mask > 1) {
    mask >>= 1;
    pinNumber++;
  }
  
  // Call the registered callback
  auto it = g_interruptHandlers.find(pinNumber);
  if (it != g_interruptHandlers.end()) {
    it->second.callback(it->second.pin, it->second.userData);
  }
}
