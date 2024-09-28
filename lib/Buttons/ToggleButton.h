#pragma once

#include "IButton.h"
#include <Arduino.h>

/**
 * 硬件上是自复位开关，软件上每次按下松开切换到下一个状态。  
   类似于按键后在不同状态之间循环切换。
   This button is a momentary switch in hardware, where each press and release cycles the software to the next state. 
   For example, pressing the button once may turn a device on, and pressing it again may turn it off.
 */
class ToggleButton : public IButton {
public:
    ToggleButton(uint8_t pin);
    
    // Implement interface methods
    void update() override;
    bool isPressed() override;
    bool isReleased() override;
    
    // ToggleButton specific methods
    bool getState() const;
    void toggle();

private:
    uint8_t _pin;
    bool _currentState;
    bool _lastReading;
    unsigned long _lastDebounceTime;
    static const unsigned long DEBOUNCE_DELAY = 500;  // milliseconds
};
