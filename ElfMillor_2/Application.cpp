#include "Application.h"

#include "SceneController.h"

#include "Player.h"
#include "Input.h"
#include "game.h"

#include "DxLib.h"
#include <cassert>

using namespace std;

Application& Application::GetInstance()
{
	// TODO: return ステートメントをここに挿入します
	static Application app;
	return app;
}

bool Application::Init()
{
	// 一部のDxLib関数はDxLib_Init()の前に呼ぶ必要がある

	// フルスクリーンではなく、ウィンドウモードで開く
	ChangeWindowMode(Game::kDefaultWindowMode);
	// 画面のサイズを変更する
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);

	SetWindowText("魔法使いの鏡");
	if (DxLib_Init())		// ＤＸライブラリ初期化処理
	{
		return false;			// エラーが起きたら直ちに終了
	}

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

void Application::Run()
{
	m_frame = 0;

	// メモリの確保
	SceneController sceneController;
	Input input;

	// ゲームループ
	while (ProcessMessage() == 0) // Windowsが行う処理を待つ
	{
		// 今回のループが始まった時間を覚えておく
		LONGLONG time = GetNowHiPerformanceCount();

		// 画面全体をクリアする
		ClearDrawScreen();

		// ここにゲームの処理を書く

		// パッドの入力更新処理
		input.Update();

		sceneController.Update(input);
		sceneController.Draw();

		// 画面の切り替わりを待つ必要がある
		ScreenFlip();
		++m_frame;

		// FPS60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}

		// もしＥＳＣキーが押されていたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

	}
}

void Application::Terminate()
{
	// ＤＸライブラリ使用の終了処理
	DxLib_End();
}

//const Size& Application::GetWindowSize() const
//{
//	return m_windowSize;
//	// TODO: return ステートメントをここに挿入します
//}