#include "Pad.h"
#include "DxLib.h"

namespace
{
	// ���݂̃t���[���̃p�b�h����
	int padInput = 0;

	// �ЂƂO�̃t���[���̃p�b�h����
	int lastInput = 0;
}

namespace Pad
{
	void Update()
	{
		// �O�̃t���[���ɉ�����Ă����{�^���̏����L�����Ă���
		lastInput = padInput;

		// �̃t���[���ɉ�����Ă���{�^���̏����擾����
		padInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool IsPress(int button)
	{
		return (padInput & button);
	}

	bool IsTrigger(int button)
	{
		bool isNow = (padInput & button);
		bool isLast = (lastInput & button);
		return (isNow && !isLast);
	}
}