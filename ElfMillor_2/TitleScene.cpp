#include "TitleScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "GameScene.h"
#include "game.h"

#include <memory>
#include <cassert>

namespace
{
	// 画面中央
	constexpr int kGameScreenHalfWidth = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;
	constexpr int kFadeInterval = 60;
}

void TitleScene::FadeInUpdate(Input&)
{
	if (m_frame-- <= 0)
	{
		m_update = &TitleScene::NormalUpdate;
		m_draw = &TitleScene::NormalDraw;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (m_frame++ >= kFadeInterval)
	{
		// このChangeSceneが呼び出された直後はTitleSceneオブジェクトは消滅している
		// この後に何か書くと、死んだメモリにアクセスしてクラッシュする
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));

		// 自分が死んでいるのでもし余計な処理が入っているとまずいのでreturn;
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_3))
	{
		m_update = &TitleScene::FadeOutUpdate;
		m_draw = &TitleScene::FadeDraw;
		m_frame = 0;
	}
}

void TitleScene::FadeDraw()
{
	DrawLine(160, 0, 160, 720, 0xffffff, true);
	DrawLine(1120, 0, 1120, 720, 0xffffff, true);
	DrawString(600, 100, "TitleScene", 0xffffff);
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	DrawString(600, 100, "TitleScene", 0xffffff);
	DrawLine(160, 0, 160, 720, 0xffffff, true);
	DrawLine(1120, 0, 1120, 720, 0xffffff, true);
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller),
	m_update(&TitleScene::FadeInUpdate),
	m_draw(&TitleScene::FadeDraw)
{
	m_frame = kFadeInterval;
}

void TitleScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void TitleScene::Draw()
{
	(this->*m_draw)();
}