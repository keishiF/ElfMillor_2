#include "Application.h"
#include "SceneController.h"
#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <string>

using namespace std;

Application& Application::GetInstance()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	static Application app;
	return app;
}

bool Application::Init()
{
	return false;
}

void Application::Run()
{
}

void Application::Terminate()
{
}

const Size& Application::GetWindowSize() const
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
}