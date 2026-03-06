# STM32 HAL Implementation

STM32-specific implementation of the `hal_interface` for embedded GPIO and interrupt control. This package provides concrete implementations of the HAL interfaces for STM32U0xx microcontrollers.

## Features

- **Complete GPIO Support**: Full implementation of `I_GPIODriver` interface
- **Interrupt Handling**: EXTI-based GPIO interrupts with callback support
- **STM32 HAL Integration**: Built on top of STM32 HAL library
- **Pin Mapping**: Automatic mapping of logical pins to STM32 GPIO ports
- **Efficient**: Direct register access through STM32 HAL for optimal performance

## Supported Hardware

- STM32U0xx family (STM32U031xx specifically)
- Easily adaptable to other STM32 families

## Implementation Details

### `GPIODriver` Class

Concrete implementation of `I_GPIODriver`:

- Automatically enables GPIO port clocks
- Configures EXTI interrupts for input pins
- Supports all GPIO modes (input, output, pull-up/down)
- Manages interrupt callbacks efficiently

### Pin Mapping

Pins are specified using the `PinID` structure:
- `port`: 0=GPIOA, 1=GPIOB, 2=GPIOC, 3=GPIOD
- `pin`: Pin number 0-15

Example:
```cpp
PinID led = {0, 5};  // PA5
PinID button = {2, 13};  // PC13
```

## Usage with vcpkg

Add to your `vcpkg.json`:

```json
{
  "dependencies": [
    "hal_interface",
    "stm_hal"
  ]
}
```

In your CMakeLists.txt:

```cmake
find_package(hal_interface CONFIG REQUIRED)
find_package(stm_hal CONFIG REQUIRED)

target_link_libraries(your_target PRIVATE 
    hal_interface::hal_interface
    stm_hal::stm_hal
)
```

## Example Usage

```cpp
import GPIO_DRIVER;

GPIODriver gpio;
gpio.Init();

// Configure LED pin
GPIOConfig ledConfig = {
    .pin = {0, 5},  // PA5
    .mode = GPIOPinMode::Output,
    .pull = GPIOPullMode::NoPull,
    .interruptTrigger = GPIOInterruptTrigger::NoInterrupt,
    .callback = nullptr,
    .callbackUserData = nullptr
};
gpio.ConfigurePin(ledConfig);

// Toggle LED
gpio.TogglePin({0, 5});
```

## C++20 Modules

This implementation uses C++20 modules:
- `GPIO_DRIVER` - Main GPIO driver module
- `PIN_DRIVER` - Pin mapping utilities

Import them in your code:
```cpp
import GPIO_DRIVER;
import PIN_DRIVER;
```

## Dependencies

- `hal_interface` - Abstract HAL interface
- STM32 HAL library (included)
- ARM GCC toolchain for embedded builds

## Building

Requires CMake 3.22+ and C++20 support with modules.

## License

See vcpkg.json for license information.
