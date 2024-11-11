#include "Input.h"
#include "DxLib.h"

Input::Input() :
	m_padState(),
	m_lastPadState()
{
}

void Input::Update()
{
	m_lastPadState = m_padState;

	GetJoypadInputState()
}

bool Input::IsTrigger(int id) const
{
	return false;
}