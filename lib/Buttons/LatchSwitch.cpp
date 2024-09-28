#include "LatchSwitch.h"
#include <Arduino.h>

LatchSwitch::LatchSwitch(uint8_t pin) : _pin(pin), _currentState(false), _lastReading(false) {
    pinMode(_pin, INPUT_PULLUP);
}

void LatchSwitch::update() {
    bool reading = digitalRead(_pin) == LOW;
    if (reading != _lastReading) {
        toggle();
    }
    _lastReading = reading;
}

bool LatchSwitch::isPressed() {
    return _currentState;
}

bool LatchSwitch::isReleased() {
    return !_currentState;
}

bool LatchSwitch::getState() const {
    return _currentState;
}

void LatchSwitch::toggle() {
    _currentState = !_currentState;
}
