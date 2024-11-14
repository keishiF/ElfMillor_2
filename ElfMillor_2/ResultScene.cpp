#include "ResultScene.h"
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
	constexpr int kGameScreenHalfWidth = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;

	constexpr int kFadeInterval = 60;
}

void ResultScene::FadeInUpdate(Input& input)
{
	if (m_frame-- <= 0)
	{
		m_update = &ResultScene::NormalUpdate;
		m_draw = &ResultScene::NormalDraw;
	}
}

void ResultScene::FadeOutUpdate(Input& input)
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

void ResultScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_3))
	{
		m_update = &ResultScene::FadeOutUpdate;
		m_draw = &ResultScene::FadeDraw;
		m_frame = 0;
	}
}

void ResultScene::FadeDraw()
{
	DrawBox(0, 0, 160, 720, 0xffffff, true);
	DrawBox(1120, 0, 1280, 720, 0xffffff, true);
	DrawString(600, 100, "ResultScene", 0xffffff);
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ResultScene::NormalDraw()
{
	DrawString(600, 100, "ResultScene", 0xffffff);
	DrawBox(0, 0, 160, 720, 0xffffff, true);
	DrawBox(1120, 0, 1280, 720, 0xffffff, true);
}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_update(&ResultScene::FadeInUpdate),
	m_draw(&ResultScene::FadeDraw)
{
	m_frame = kFadeInterval;
}

void ResultScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void ResultScene::Draw()
{
	(this->*m_draw)();
}