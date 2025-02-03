#include "ClearScene.h"
#include "TitleScene.h"
#include "SceneController.h"

#include "game.h"
#include "Input.h"

#include "DxLib.h"
#include <memory>
#include <cassert>

namespace
{
	// 画面中央
	constexpr int kGameScreenHalfWidth = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;

	constexpr int kFadeInterval = 60;
}

ClearScene::ClearScene(SceneController& controller, int finalScore) :
	SceneBase(controller),
	m_finalScore(finalScore),
	m_update(&ClearScene::FadeInUpdate),
	m_draw(&ClearScene::FadeDraw),
	m_handle(-1),
	m_fontHandle(-1)
{
	m_fontHandle = CreateFontToHandle("Algerian", 64, -1, DX_FONTTYPE_ANTIALIASING_8X8);

	m_handle = LoadGraph("data/image/BackGround/GameClear.png");
	assert(m_handle != -1);

	m_fadeFrameCount = kFadeInterval;
}

void ClearScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void ClearScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_3))
	{
		m_update = &ClearScene::FadeOutUpdate;
		m_draw = &ClearScene::FadeDraw;
		m_fadeFrameCount = 0;
	}
}

void ClearScene::FadeInUpdate(Input&)
{
	if (m_fadeFrameCount-- <= 0)
	{
		m_update = &ClearScene::NormalUpdate;
		m_draw = &ClearScene::NormalDraw;
	}
}

void ClearScene::FadeOutUpdate(Input&)
{
	if (m_fadeFrameCount++ >= kFadeInterval)
	{
		// このChangeSceneが呼び出された直後はTitleSceneオブジェクトは消滅している
		// この後に何か書くと、死んだメモリにアクセスしてクラッシュする
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));

		// 自分が死んでいるのでもし余計な処理が入っているとまずいのでreturn;
		return;
	}
}

void ClearScene::Draw()
{
	(this->*m_draw)();
}

void ClearScene::NormalDraw()
{
	DrawGraph(0, 0, m_handle, true);

	if (m_finalScore > 40000)
	{
		DrawFormatStringToHandle(475, 400, 0xffffff, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(730, 400, 0xffedab, m_fontHandle, "S");
		DrawFormatStringToHandle(375, 400, 0xffffff, m_fontHandle, "\nScore:%d", m_finalScore);
	}
	else if (m_finalScore > 35000)
	{
		DrawFormatStringToHandle(475, 400, 0xffffff, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(730, 400, 0xff0000, m_fontHandle, "A");
		DrawFormatStringToHandle(375, 400, 0xffffff, m_fontHandle, "\nScore:%d", m_finalScore);
	}
	else if (m_finalScore > 30000)
	{
		DrawFormatStringToHandle(475, 400, 0xffffff, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(730, 400, 0x0000ff, m_fontHandle, "B");
		DrawFormatStringToHandle(375, 400, 0xffffff, m_fontHandle, "\nScore:%d", m_finalScore);
	}
	else if (m_finalScore > 25000)
	{
		DrawFormatStringToHandle(475, 400, 0xffffff, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(730, 400, 0x00ff00, m_fontHandle, "C");
		DrawFormatStringToHandle(375, 400, 0xffffff, m_fontHandle, "\nScore:%d", m_finalScore);
	}
	else
	{
		DrawFormatStringToHandle(475, 400, 0xffffff, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(730, 400, 0xff00ff, m_fontHandle, "D");
		DrawFormatStringToHandle(375, 400, 0xffffff, m_fontHandle, "\nScore:%d", m_finalScore);
	}
}

void ClearScene::FadeDraw()
{
	DrawGraph(0, 0, m_handle, true);

	float rate = static_cast<float>(m_fadeFrameCount) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}