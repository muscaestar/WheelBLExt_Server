#include "ToggleButton.h"
#include <Arduino.h>

ToggleButton::ToggleButton(uint8_t pin) : _pin(pin), _currentState(false), _lastReading(false), _lastDebounceTime(0) {
    pinMode(_pin, INPUT_PULLUP);
}

void ToggleButton::update() {
    bool reading = digitalRead(_pin) == LOW;
    
    if ((millis() - _lastDebounceTime) > DEBOUNCE_DELAY) { // 防抖，如果两次变化间隔小于50ms，则认为是抖动，不处理
        if (reading != _lastReading) { // 物理信号变化了
            _lastDebounceTime = millis(); // 更新时间戳
        }
        if (reading) {
            toggle(); // 切换状态
        }
    }

    _lastReading = reading;
}

bool ToggleButton::isPressed() {
    return _currentState;
}

bool ToggleButton::isReleased() {
    return !_currentState;
}

bool ToggleButton::getState() const {
    return _currentState;
}

void ToggleButton::toggle() {
    _currentState = !_currentState;
}
