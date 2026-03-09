#include "stm-hal/system_driver.hpp"
#include "stm32u0xx_hal.h"

// Initialize the STM32 HAL
void SystemDriver::Init() {
  HAL_Init();
}

// Configure STM32 system clock
void SystemDriver::ConfigureClock() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure the main internal regulator output voltage
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

  // Initializes the RCC Oscillators
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    // Error handling - infinite loop (could be enhanced)
    while(1);
  }

  // Initializes the CPU, AHB and APB buses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    // Error handling - infinite loop (could be enhanced)
    while(1);
  }
}

// Get system tick in milliseconds
uint32_t SystemDriver::GetTick() {
  return HAL_GetTick();
}

// Blocking delay in milliseconds
void SystemDriver::Delay(uint32_t ms) {
  HAL_Delay(ms);
}

// Reset the system
void SystemDriver::Reset() {
  NVIC_SystemReset();
}

// Enter low-power mode and wait for interrupt
void SystemDriver::WaitForInterrupt() {
  __WFI();
}
