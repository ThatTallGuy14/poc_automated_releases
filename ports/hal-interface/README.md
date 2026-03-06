# HAL Interface

Generic hardware abstraction layer interface for embedded systems. This package provides C++ abstract interfaces for GPIO operations, enabling hardware-agnostic embedded applications.

## Features

- **Hardware Agnostic**: Pure interface definitions with no hardware dependencies
- **GPIO Operations**: Pin configuration, read/write, toggle operations
- **Interrupt Support**: Callback-based interrupt handling for GPIO pins
- **Configuration Driven**: Flexible pin configuration with pull-up/down and trigger modes

## Interface Overview

### `I_GPIODriver`

Abstract base class for GPIO operations:

- `Init()` - Initialize the GPIO driver
- `ConfigurePin(const GPIOConfig&)` - Configure a pin with full settings
- `SetPin(PinID, GPIOPinState)` - Set output pin state
- `ReadPin(PinID)` - Read input pin state
- `TogglePin(PinID)` - Toggle output pin state
- `EnableInterrupt(PinID, bool)` - Enable/disable pin interrupts

### `I_SystemDriver`

Abstract base class for system control:

- `Init()` - Initialize the HAL
- `ConfigureClock()` - Configure system clock
- `GetTick()` - Get system tick in milliseconds
- `Delay(ms)` - Blocking delay
- `Reset()` - System reset

### Data Structures

```cpp
// Pin identifier
struct PinID {
    uint8_t port;  // Port number (0=A, 1=B, etc.)
    uint8_t pin;   // Pin number (0-15)
};

// Pin configuration
struct GPIOConfig {
    PinID pin;
    GPIOPinMode mode;              // Input or Output
    GPIOPullMode pull;             // NoPull, PullUp, PullDown
    GPIOInterruptTrigger trigger;  // Interrupt configuration
    GPIOInterruptCallback callback; // Interrupt handler
    void* callbackUserData;        // User data for callback
};
```

## Usage with vcpkg

Add to your `vcpkg.json`:

```json
{
  "dependencies": ["hal-interface"]
}
```

In your CMakeLists.txt:

```cmake
find_package(hal-interface CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE hal-interface::hal-interface)
```

## Implementation

This package only provides interfaces. You must link against a concrete implementation such as `stm-hal` to use these interfaces.

## License

See vcpkg.json for license information.
