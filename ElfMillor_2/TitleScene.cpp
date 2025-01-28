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

TitleScene::TitleScene(SceneController& controller) :
	SceneBase(controller),
	m_update(&TitleScene::FadeInUpdate),
	m_draw(&TitleScene::FadeDraw),
	m_handle(-1)
{
	m_frame = kFadeInterval;

	m_handle = LoadGraph("data/image/BackGround/title2.png");
}

void TitleScene::Update(Input& input)
{
	(this->*m_update)(input);
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

void TitleScene::FadeInUpdate(Input&)
{
	if (m_frame-- <= 0)
	{
		m_update = &TitleScene::NormalUpdate;
		m_draw   = &TitleScene::NormalDraw;
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

void TitleScene::Draw()
{
	(this->*m_draw)();
}

void TitleScene::NormalDraw()
{
	DrawGraph(0, 0, m_handle, true);
}

void TitleScene::FadeDraw()
{
	DrawGraph(0, 0, m_handle, true);

	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}