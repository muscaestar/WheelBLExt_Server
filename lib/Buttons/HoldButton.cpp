#include "HoldButton.h"
#include <Arduino.h>

HoldButton::HoldButton(uint8_t pin) : _pin(pin), _currentState(false) {
    pinMode(_pin, INPUT_PULLUP);
}

void HoldButton::update() {
    _currentState = digitalRead(_pin) == LOW;
}

bool HoldButton::isPressed() {
    return _currentState;
}

bool HoldButton::isReleased() {
    return !_currentState;
}
