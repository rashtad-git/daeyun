#pragma once

class InputManager
{
public:
    InputManager();
    ~InputManager();

public:
    void Update(double deltaTime);

private:
    struct PrivateData;
    PrivateData *This;
};
