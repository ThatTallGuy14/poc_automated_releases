/**
 * @file blinky_feature.cpp
 * @brief Blinky feature implementation - LED control via button interrupt
 * 
 * This provides a reusable blinky feature that can be integrated into
 * any firmware. It's hardware-agnostic and uses only HAL interfaces.
 * 
 * Features:
 * - Uses generic HAL interface (hal-interface)
 * - Hardware-agnostic design
 * - Interrupt-driven button handling
 * - Configurable LED and button pins
 * - Feature can be initialized and started by consumer firmware
 */

#include "blinky/blinky_feature.hpp"

// Implementation of BlinkyFeature methods

BlinkyFeature::BlinkyFeature(I_GPIODriver& gpioDriver, const BlinkyConfig& config)
    : m_gpioDriver(gpioDriver), m_config(config), m_ledState(false), m_running(false) {
}

BlinkyFeature::~BlinkyFeature() {
    Stop();
}

void BlinkyFeature::Start() {
    if (m_running) {
        return; // Already running
    }
    
    // Configure LED pin as output
    GPIOConfig ledConfig = {
        .pin = m_config.ledPin,
        .mode = GPIOPinMode::Output,
        .pull = GPIOPullMode::NoPull,
        .interruptTrigger = GPIOInterruptTrigger::NoInterrupt,
        .callback = nullptr,
        .callbackUserData = nullptr
    };
    m_gpioDriver.ConfigurePin(ledConfig);
    
    // Set initial LED state
    m_ledState = false;
    m_gpioDriver.SetPin(m_config.ledPin, GPIOPinState::Low);
    
    // Configure button pin as input with interrupt
    GPIOConfig buttonConfig = {
        .pin = m_config.buttonPin,
        .mode = GPIOPinMode::Input,
        .pull = m_config.buttonPull,
        .interruptTrigger = m_config.buttonTrigger,
        .callback = &BlinkyFeature::ButtonInterruptCallback,
        .callbackUserData = this
    };
    m_gpioDriver.ConfigurePin(buttonConfig);
    
    m_running = true;
}

void BlinkyFeature::Stop() {
    if (!m_running) {
        return;
    }
    
    // Disable button interrupt
    m_gpioDriver.EnableInterrupt(m_config.buttonPin, false);
    
    // Turn off LED
    m_gpioDriver.SetPin(m_config.ledPin, GPIOPinState::Low);
    m_ledState = false;
    
    m_running = false;
}

bool BlinkyFeature::IsRunning() const {
    return m_running;
}

bool BlinkyFeature::GetLedState() const {
    return m_ledState;
}

void BlinkyFeature::SetLedState(bool on) {
    m_ledState = on;
    m_gpioDriver.SetPin(m_config.ledPin, on ? GPIOPinState::High : GPIOPinState::Low);
}

void BlinkyFeature::ToggleLed() {
    SetLedState(!m_ledState);
}

void BlinkyFeature::ButtonInterruptCallback(PinID pin, void* userData) {
    // Cast user data back to BlinkyFeature instance
    BlinkyFeature* blinky = static_cast<BlinkyFeature*>(userData);
    
    if (blinky != nullptr && blinky->m_running) {
        // Toggle LED on button press
        blinky->ToggleLed();
    }
}
