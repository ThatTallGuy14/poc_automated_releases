#ifndef I_SYSTEM_H
#define I_SYSTEM_H

#include <stdint.h>

/**
 * @brief System initialization and control interface
 * 
 * Provides hardware-agnostic system initialization and control methods.
 * Implementations handle vendor-specific HAL initialization, clock
 * configuration, and system control functions.
 */
class I_SystemDriver {
public:
  I_SystemDriver() {}
  virtual ~I_SystemDriver() {}
  
  /**
   * @brief Initialize the hardware abstraction layer
   * 
   * Must be called before using any other HAL functionality.
   * Performs vendor-specific HAL initialization.
   */
  virtual void Init() = 0;
  
  /**
   * @brief Configure the system clock
   * 
   * Sets up the system clock tree to the desired frequency.
   * Implementation is hardware-specific.
   */
  virtual void ConfigureClock() = 0;
  
  /**
   * @brief Provides a tick value in milliseconds
   * 
   * @return Current system tick count in milliseconds
   */
  virtual uint32_t GetTick() = 0;
  
  /**
   * @brief Blocking delay for specified milliseconds
   * 
   * @param ms Number of milliseconds to delay
   */
  virtual void Delay(uint32_t ms) = 0;
  
  /**
   * @brief System reset
   * 
   * Performs a software system reset.
   */
  virtual void Reset() = 0;
  
  /**
   * @brief Enter low-power mode and wait for interrupt
   * 
   * Puts the processor into sleep mode until an interrupt occurs.
   * This is useful in the main loop to reduce power consumption.
   */
  virtual void WaitForInterrupt() = 0;
};

#endif // I_SYSTEM_H
