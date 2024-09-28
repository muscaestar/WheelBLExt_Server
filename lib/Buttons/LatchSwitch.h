#pragma once

#include "IButton.h"
#include <Arduino.h>

/**
 * 硬件上是非自复位开关，软件上切换到导通和断开的状态。  
   类似于开关机按钮，按下后状态保持直到再次按下。
   This button is a non-momentary switch in hardware. In the software, closing the switch triggers one state, and opening it triggers another. 
   It behaves like a typical toggle switch, where the state persists until manually changed.
 */
class LatchSwitch : public IButton {
public:
    LatchSwitch(uint8_t pin);
    
    void update() override;
    bool isPressed() override;
    bool isReleased() override;

    bool getState() const;
    void toggle();

private:
    uint8_t _pin;
    bool _currentState;
    bool _lastReading;
};

