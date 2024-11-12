#include "Application.h"
#include "SceneController.h"
#include "Input.h"
#include "game.h"
#include "Player.h"
#include <DxLib.h>
#include <cassert>

using namespace std;

Application& Application::GetInstance()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	static Application app;
	return app;
}

bool Application::Init()
{
	// �ꕔ��DxLib�֐���DxLib_Init()�̑O�ɌĂԕK�v������

	// �t���X�N���[���ł͂Ȃ��A�E�B���h�E���[�h�ŊJ��
	ChangeWindowMode(Game::kDefaultWindowMode);
	// ��ʂ̃T�C�Y��ύX����
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	return false;
}

void Application::Run()
{
	m_frame = 0;
	SceneController sceneController;
	Input input;

	// �Q�[�����[�v
	while (ProcessMessage() == 0) // Windows���s��������҂�
	{
		// ����̃��[�v���n�܂������Ԃ��o���Ă���
		LONGLONG time = GetNowHiPerformanceCount();

		// ��ʑS�̂��N���A����
		ClearDrawScreen();

		// �����ɃQ�[���̏���������

		// �p�b�h�̓��͍X�V����
		input.Update();

		sceneController.Update(input);
		sceneController.Draw();

		// ��ʂ̐؂�ւ���҂K�v������
		ScreenFlip();
		++m_frame;

		// FPS60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}
}

void Application::Terminate()
{
	// �c�w���C�u�����g�p�̏I������
	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	return m_windowSize;
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
}