#include "GameOverScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "TitleScene.h"
#include "game.h"

#include <memory>
#include <cassert>

namespace
{
	// 画面中央
	constexpr int kGameScreenHalfWidth  = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;

	constexpr int kFadeInterval = 60;
}

void GameOverScene::FadeInUpdate(Input& input)
{
	if (m_frame-- <= 0)
	{
		m_update = &GameOverScene::NormalUpdate;
		m_draw = &GameOverScene::NormalDraw;
	}
}

void GameOverScene::FadeOutUpdate(Input& input)
{
	if (m_frame++ >= kFadeInterval)
	{
		// このChangeSceneが呼び出された直後はTitleSceneオブジェクトは消滅している
		// この後に何か書くと、死んだメモリにアクセスしてクラッシュする
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));

		// 自分が死んでいるのでもし余計な処理が入っているとまずいのでreturn;
		return;
	}
}

void GameOverScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_3))
	{
		m_update = &GameOverScene::FadeOutUpdate;
		m_draw   = &GameOverScene::FadeDraw;
		m_frame  = 0;
	}
}

void GameOverScene::FadeDraw()
{
	DrawGraph(0, 0, m_handle, true);

	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameOverScene::NormalDraw()
{
	DrawGraph(0, 0, m_handle, true);
}

GameOverScene::GameOverScene(SceneController& controller):
	SceneBase(controller),
	m_update(&GameOverScene::FadeInUpdate),
	m_draw(&GameOverScene::FadeDraw),
	m_handle(-1)
{
	m_frame = kFadeInterval;
	m_handle = LoadGraph("img/BackGround/GameOver.png");
}

void GameOverScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameOverScene::Draw()
{
	(this->*m_draw)();
}