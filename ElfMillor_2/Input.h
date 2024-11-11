#pragma once
class Input
{
private:
	struct PadValue
	{
		char value[256];
	};

	PadValue m_padState;

	PadValue m_lastPadState;

public:
	Input();

	void Update();

	bool IsTrigger(int id)const;
};