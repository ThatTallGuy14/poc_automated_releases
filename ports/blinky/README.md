# Blinky Feature Library

A hardware-agnostic interrupt-driven LED control feature library. This is a reusable component that consumers integrate into their firmware.

## Architecture

Blinky is a **feature library**, not a standalone application:
- Provides `BlinkyFeature` class
- Consumer firmware creates and initializes it
- Uses dependency injection for HAL drivers
- No vendor-specific code - only HAL interfaces

## Features

- **Hardware Agnostic**: Uses only `hal_interface` abstractions
- **Interrupt Driven**: Button press handled via GPIO interrupts
- **Configurable**: Flexible configuration structure
- **Reusable**: Can instantiate multiple instances
- **Library**: Consumer controls initialization and lifecycle

## Functionality

- Provides `BlinkyFeature` class for integration
- Button press (configurable edge) toggles LED state
- LED responds immediately via interrupt, no polling
- Configurable LED and button pins via `BlinkyConfig`
- Start/stop methods for lifecycle control
- Manual LED control methods available

## Default Pin Configuration

Defined in `blinky_config.hpp`:

- **LED Pin**: PA5 (GPIOA, Pin 5)
- **Button Pin**: PC13 (GPIOC, Pin 13) - with pull-up

### Customizing Pins

Override defaults by defining constants before including config:

```cpp
#define LED_PIN_PORT 1      // GPIOB
#define LED_PIN_NUMBER 0    // PB0
#define BUTTON_PIN_PORT 0   // GPIOA
#define BUTTON_PIN_NUMBER 0 // PA0

#include "blinky_config.hpp"
```

## Usage in Consumer Firmware

### 1. Add Dependency

Add to your firmware's `vcpkg.json`:

```json
{
  "dependencies": [
    "hal_interface",
    "stm_hal",      // Or your HAL implementation
    "blinky"
  ]
}
```

### 2. Consumer Firmware Pattern

```cpp
#include "blinky_feature.hpp"
import GPIO_DRIVER;
import SYSTEM_DRIVER;

int main() {
    // Consumer initializes system
    SystemDriver sys;
    sys.Init();
    sys.ConfigureClock();
    
    // Consumer initializes HAL drivers
    GPIODriver gpio;
    gpio.Init();
    
    // Consumer creates and starts blinky feature
    BlinkyFeature blinky(gpio, DEFAULT_BLINKY_CONFIG);
    blinky.Start();
    
    // Consumer's main loop
    while(1) {
        sys.Delay(100);
        // Your application logic
    }
}
```

## Building with vcpkg

```bash
# Install dependencies
vcpkg install blinky --overlay-ports=./ports

# Build your project
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

## Code Structure

```
blinky/
├── src/
│   └── main.cpp              # Application entry point
├── include/
│   └── blinky_config.hpp     # Pin configuration
├── CMakeLists.txt            # Build configuration
├── vcpkg.json               # Package dependencies
└── portfile.cmake           # vcpkg port file
```

## Application Flow

1. **Initialization**
   - HAL_Init() - Initialize STM32 HAL
   - SystemClock_Config() - Configure system clock
   - InitializeGPIO() - Setup LED (output) and Button (input with interrupt)

2. **Main Loop**
   - Minimal processing, HAL_Delay(10)
   - LED control handled by interrupt callback

3. **Interrupt Handler**
   - Button press triggers `ButtonInterruptCallback`
   - Callback toggles LED state
   - Fast and responsive

## Example Usage as a Library

You can use blinky as a reference or library:

```cpp
#include "i_gpio.hpp"
#include "blinky_config.hpp"
import GPIO_DRIVER;

void MyButtonCallback(PinID pin, void* userData) {
    // Your custom handler
}

int main() {
    GPIODriver gpio;
    gpio.Init();
    
    // Configure with custom callback
    GPIOConfig buttonConfig = {
        .pin = BUTTON_PIN,
        .mode = GPIOPinMode::Input,
        .pull = GPIOPullMode::PullUp,
        .interruptTrigger = GPIOInterruptTrigger::FallingEdge,
        .callback = MyButtonCallback,
        .callbackUserData = nullptr
    };
    gpio.ConfigurePin(buttonConfig);
    
    while(1) {
        // Your application logic
    }
}
```

## Dependencies

- `hal_interface` (required) - Abstract HAL interface
- `stm_hal` (optional) - STM32 implementation, or provide your own

## Hardware Requirements

- STM32U031xx microcontroller (or adapt for your MCU)
- LED connected to configured pin (e.g., PA5)
- Button connected to configured pin (e.g., PC13)
- ARM GCC toolchain for embedded builds

## Extending

This application can be extended for:
- Multiple buttons and LEDs
- Different interrupt triggers (rising, falling, both)
- Advanced debouncing logic
- State machines and complex behaviors
- Low-power sleep modes

## Building for Other Targets

- Implement `I_GPIODriver` for your target hardware
- Update pin definitions in `blinky_config.hpp`
- Modify `SystemClock_Config()` for your clock tree
- Update linker script and startup file

## License

See vcpkg.json for license information.
  