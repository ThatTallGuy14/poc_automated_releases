#ifndef BLINKY_FEATURE_HPP
#define BLINKY_FEATURE_HPP

#include "hal-interface/i_gpio.hpp"
#include "hal-interface/i_generic.hpp"
#include "blinky/blinky_config.hpp"

class BlinkyFeature {
public:

    BlinkyFeature(I_GPIODriver& gpioDriver, const BlinkyConfig& config);
    

    ~BlinkyFeature();
    

    void Start();


    void Stop();
    

    bool IsRunning() const;

    bool GetLedState() const;
    

    void SetLedState(bool on);

    void ToggleLed();

private:

    static void ButtonInterruptCallback(PinID pin, void* userData);
    
    I_GPIODriver& m_gpioDriver;     
    BlinkyConfig m_config;          
    bool m_ledState;                
    bool m_running;                 
};

#endif // BLINKY_FEATURE_HPP
