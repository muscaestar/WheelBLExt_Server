#pragma once

#include "IButton.h"
#include <Arduino.h>


/**
 * 硬件上是自复位开关，软件上按住时维持某个状态，松开时切换到另一个状态。  
   这种按钮通常用于按住时触发某个功能，松开后停止该功能。
   This button is also a momentary switch in hardware, but in the software, holding the button activates one state, and releasing it switches to another state. 
   It’s commonly used for actions that need to be held down, such as triggering a temporary mode.
 */
class HoldButton : public IButton {
public:
    HoldButton(uint8_t pin);
    
    void update() override;
    bool isPressed() override;
    bool isReleased() override;

private:
    uint8_t _pin;
    bool _currentState;
};
