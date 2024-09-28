#include "RangeButton.h"
#include <Arduino.h>

RangeButton::RangeButton(uint8_t pin, int lowerThreshold, int upperThreshold)
    : _pin(pin), _lowerThreshold(lowerThreshold), _upperThreshold(upperThreshold), _currentValue(0), _isInRange(false) {
    pinMode(_pin, INPUT);
}

void RangeButton::update() {
    _currentValue = analogRead(_pin);
    _isInRange = (_currentValue >= _lowerThreshold && _currentValue <= _upperThreshold);
}

bool RangeButton::isPressed() {
    return _isInRange;
}

bool RangeButton::isReleased() {
    return !_isInRange;
}

int RangeButton::getValue() const {
    return _currentValue;
}
