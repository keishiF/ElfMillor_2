#include "DxLib.h"

#include "Application.h"

using namespace std;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application& app = Application::GetInstance();

	// �A�v���P�[�V�����̏�����
	if (!app.Init())
	{
		return -1;
	}

	// ���C�����[�v
	app.Run();

	// �㏈��
	app.Terminate();

	DxLib_End();

	return 0;
}