#pragma once

class IButton {
public:
    virtual ~IButton() = default;

    virtual void update() = 0;
    virtual bool isPressed() = 0;
    virtual bool isReleased() = 0;

    // Additional common button interface methods can be added here
};
