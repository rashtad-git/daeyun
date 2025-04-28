#pragma once

class Node;

class StateManager
{
public:
	StateManager();
    ~StateManager();

public:
	void Init();
    void Update(double deltaTime);

private:
    struct PrivateData;
    PrivateData *This;
};
