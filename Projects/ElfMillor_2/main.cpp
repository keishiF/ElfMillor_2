#include "DxLib.h"

#include "Application.h"

using namespace std;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application& app = Application::GetInstance();

	// アプリケーションの初期化
	if (!app.Init())
	{
		return -1;
	}

	// メインループ
	app.Run();

	// 後処理
	app.Terminate();

	DxLib_End();

	return 0;
}