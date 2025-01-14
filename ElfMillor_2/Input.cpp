#include "Input.h"
#include "DxLib.h"

Input::Input():
	m_padInput(),
	m_lastInput()
{
}

void Input::Update()
{
	m_lastInput = m_padInput;

	m_padInput  = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}

bool Input::IsPress(int button)
{
	return (m_padInput & button);
}

bool Input::IsTrigger(int button)
{
	// (padInput & button) == 0‚Ìê‡false
	// ‚»‚êˆÈŠO‚Ìê‡‚Ítrue‚ª“ü‚é
	bool isNow  = (m_padInput & button);
	bool isLast = (m_lastInput & button);
	return (isNow && !isLast);
}