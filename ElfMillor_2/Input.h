#pragma once

class Input
{
public:
	Input();

	void Update();

	bool IsPress(int button);

	bool IsTrigger(int button);
private:
	int m_padInput;
	int m_lastInput;
};