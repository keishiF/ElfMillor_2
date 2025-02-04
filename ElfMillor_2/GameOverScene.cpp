#include "GameOverScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneController.h"

#include "game.h"
#include "Input.h"

#include "DxLib.h"
#include <memory>
#include <cassert>

namespace
{
	// 画面中央
	constexpr int kGameScreenHalfWidth  = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;

	constexpr int kFadeInterval = 60;
}

GameOverScene::GameOverScene(SceneController& controller) :
	SceneBase(controller),
	m_update(&GameOverScene::FadeInUpdate),
	m_draw(&GameOverScene::FadeDraw),
	m_handle(-1)
{
	m_fadeFrameCount = kFadeInterval;
	m_handle = LoadGraph("data/image/BackGround/GameOver.png");
}

void GameOverScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameOverScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_3))
	{
		m_update = &GameOverScene::FadeOutUpdate;
		m_draw = &GameOverScene::FadeDraw;
		m_fadeFrameCount = 0;
	}
}

void GameOverScene::FadeInUpdate(Input& input)
{
	if (m_fadeFrameCount-- <= 0)
	{
		m_update = &GameOverScene::NormalUpdate;
		m_draw = &GameOverScene::NormalDraw;
	}
}

void GameOverScene::FadeOutUpdate(Input& input)
{
	if (m_fadeFrameCount++ >= kFadeInterval)
	{
		// このChangeSceneが呼び出された直後はTitleSceneオブジェクトは消滅している
		// この後に何か書くと、死んだメモリにアクセスしてクラッシュする
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));

		// 自分が死んでいるのでもし余計な処理が入っているとまずいのでreturn;
		return;
	}
}

void GameOverScene::Draw()
{
	(this->*m_draw)();
}

void GameOverScene::NormalDraw()
{
	DrawGraph(0, 0, m_handle, true);
}

void GameOverScene::FadeDraw()
{
	DrawGraph(0, 0, m_handle, true);

	float rate = static_cast<float>(m_fadeFrameCount) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}