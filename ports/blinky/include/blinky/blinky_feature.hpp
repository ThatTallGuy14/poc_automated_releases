#ifndef BLINKY_FEATURE_HPP
#define BLINKY_FEATURE_HPP

#include "hal_interface/i_gpio.hpp"
#include "hal_interface/i_generic.hpp"
#include "blinky/blinky_config.hpp"

/**
 * @brief Blinky Feature - Interrupt-driven LED control
 * 
 * This class provides a reusable blinky feature that can be integrated into
 * any firmware. It accepts a GPIO driver via dependency injection and is
 * completely hardware-agnostic.
 * 
 * Usage:
 * 1. Create instance with GPIO driver and configuration
 * 2. Call Start() to initialize and enable the feature
 * 3. Feature handles button interrupts and toggles LED
 * 4. Call Stop() to disable the feature
 * 
 * Example:
 * @code
 * GPIODriver gpio;
 * gpio.Init();
 * 
 * BlinkyFeature blinky(gpio, DEFAULT_BLINKY_CONFIG);
 * blinky.Start();
 * 
 * // Feature now running, LED toggles on button press
 * 
 * blinky.Stop();
 * @endcode
 */
class BlinkyFeature {
public:
    /**
     * @brief Construct a new Blinky Feature
     * 
     * @param gpioDriver Reference to GPIO driver (must remain valid)
     * @param config Configuration for LED and button pins
     */
    BlinkyFeature(I_GPIODriver& gpioDriver, const BlinkyConfig& config);
    
    /**
     * @brief Destroy the Blinky Feature
     * 
     * Automatically stops the feature if running.
     */
    ~BlinkyFeature();
    
    /**
     * @brief Start the blinky feature
     * 
     * Configures GPIO pins and enables button interrupt.
     * LED will toggle on button press.
     */
    void Start();
    
    /**
     * @brief Stop the blinky feature
     * 
     * Disables button interrupt and turns off LED.
     */
    void Stop();
    
    /**
     * @brief Check if feature is currently running
     * 
     * @return true if running, false otherwise
     */
    bool IsRunning() const;
    
    /**
     * @brief Get current LED state
     * 
     * @return true if LED is on, false if off
     */
    bool GetLedState() const;
    
    /**
     * @brief Set LED state directly
     * 
     * @param on true to turn LED on, false to turn off
     */
    void SetLedState(bool on);
    
    /**
     * @brief Toggle LED state
     * 
     * Switches LED from on to off or off to on.
     */
    void ToggleLed();

private:
    /**
     * @brief Button interrupt callback (static)
     * 
     * Called by GPIO driver when button interrupt occurs.
     * 
     * @param pin Pin that triggered interrupt
     * @param userData Pointer to BlinkyFeature instance
     */
    static void ButtonInterruptCallback(PinID pin, void* userData);
    
    I_GPIODriver& m_gpioDriver;     ///< Reference to GPIO driver
    BlinkyConfig m_config;           ///< Feature configuration
    bool m_ledState;                 ///< Current LED state
    bool m_running;                  ///< Feature running status
};

#endif // BLINKY_FEATURE_HPP
