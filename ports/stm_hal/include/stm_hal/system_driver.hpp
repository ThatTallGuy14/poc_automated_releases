#ifndef SYSTEM_DRIVER_HPP
#define SYSTEM_DRIVER_HPP

#include "stm32u0xx_hal.h"
#include "hal_interface/i_system.hpp"

/**
 * @brief STM32 implementation of system driver interface
 */
class SystemDriver : public I_SystemDriver {
public:
  SystemDriver() {}
  virtual ~SystemDriver() {}
  
  /**
   * @brief Initialize the STM32 HAL
   */
  virtual void Init() override;
  
  /**
   * @brief Configure STM32 system clock
   */
  virtual void ConfigureClock() override;
  
  /**
   * @brief Get system tick in milliseconds
   */
  virtual uint32_t GetTick() override;
  
  /**
   * @brief Blocking delay in milliseconds
   */
  virtual void Delay(uint32_t ms) override;
  
  /**
   * @brief Reset the system
   */
  virtual void Reset() override;
  
  /**
   * @brief Enter low-power mode and wait for interrupt
   */
  virtual void WaitForInterrupt() override;
};

#endif // SYSTEM_DRIVER_HPP
