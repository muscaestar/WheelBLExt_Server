#pragma once

#include "IButton.h"
#include <Arduino.h>

/**
 * 这种按钮根据模拟信号的范围来确定状态。当模拟信号在特定范围内时为一个状态，超出范围时切换到另一个状态。
 * This button uses a variable resistor in hardware. The software monitors the analog signal, and when the signal falls within a certain range, it triggers one state. When the signal is outside of that range, it switches to another state. 
 * This type of button is useful for scenarios like threshold-based controls.
 */
class RangeButton : public IButton {
public:
    RangeButton(uint8_t pin, int lowerThreshold, int upperThreshold);
    
    void update() override;
    bool isPressed() override;
    bool isReleased() override;

    int getValue() const;

private:
    uint8_t _pin;
    int _lowerThreshold;
    int _upperThreshold;
    int _currentValue;
    bool _isInRange;
};
