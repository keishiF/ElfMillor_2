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

	// ボタンの座標
	constexpr int kTitleStringPosX = 330;
	constexpr int kTitleStringPosY = 560;

	// 背景の表示座標
	constexpr int kBgDrawWidthOffset = -100;
	constexpr int kBgDrawHeightOffset = -75;
}

ClearScene::ClearScene(SceneController& controller, float finalScore) :
	SceneBase(controller),
	m_blinkFrameCount(0),
	m_finalScore(finalScore),
	m_update(&ClearScene::FadeInUpdate),
	m_draw(&ClearScene::FadeDraw),
	m_bgHandle(-1),
	m_clearBgHandle(-1),
	m_fontHandle(-1),
	m_seHandle(-1),
	m_bgmHandle(-1)
{
	m_bgHandle = LoadGraph("data/image/BackGround/BackGround3.png");
	assert(m_bgHandle != -1);

	m_fontHandle = CreateFontToHandle("Algerian", 48, -1, DX_FONTTYPE_ANTIALIASING_8X8);
	assert(m_fontHandle != -1);

	m_clearBgHandle = LoadGraph("data/image/BackGround/GameClear.png");
	assert(m_clearBgHandle != -1);

	m_fadeFrameCount = kFadeInterval;

	m_seHandle = LoadSoundMem("data/sound/SE/ClearButtonSE.mp3");
	assert(m_seHandle != -1);

	// BGMの読み込み
	m_bgmHandle = LoadSoundMem("data/sound/BGM/ClearBGM2.mp3");
	assert(m_bgmHandle != -1);
}

ClearScene::~ClearScene()
{
	DeleteFontToHandle(m_fontHandle);
	DeleteGraph(m_clearBgHandle);
	DeleteSoundMem(m_seHandle);
	DeleteSoundMem(m_bgmHandle);
}

void ClearScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void ClearScene::NormalUpdate(Input& input)
{
	++m_blinkFrameCount;

	if (input.IsPress(CheckHitKeyAll()))
	{
		PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK, true);
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

		// BGMの再生
		PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
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
	DrawGraph(kBgDrawWidthOffset, kBgDrawHeightOffset, m_bgHandle, true);
	DrawGraph(0, 0, m_clearBgHandle, true);

	// 点滅効果のための条件
	if ((m_blinkFrameCount / 30) % 2 == 0)
	{
		// 文字を描画
		DrawStringToHandle(kTitleStringPosX, kTitleStringPosY, "PRESS ANY BUTTON\n　　　　　　　　　Go Title", 0xa0d8ef, m_fontHandle);
	}

	if (m_finalScore >= 40000)
	{
		DrawFormatStringToHandle(458, 400, 0xfdeff2, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(655, 400, 0xffedab, m_fontHandle, "S");
		DrawFormatStringToHandle(435, 400, 0xfdeff2, m_fontHandle, "\nScore:");
		DrawFormatStringToHandle(650, 400, 0xffedab, m_fontHandle, "\n%.0f", m_finalScore);
	}
	else if (m_finalScore >= 35000)
	{
		DrawFormatStringToHandle(458, 400, 0xfdeff2, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(655, 400, 0xcc0033, m_fontHandle, "A");
		DrawFormatStringToHandle(435, 400, 0xfdeff2, m_fontHandle, "\nScore:");
		DrawFormatStringToHandle(650, 400, 0xcc0033, m_fontHandle, "\n%.0f", m_finalScore);
	}
	else if (m_finalScore >= 30000)
	{
		DrawFormatStringToHandle(458, 400, 0xfdeff2, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(655, 400, 0x00ccff, m_fontHandle, "B");
		DrawFormatStringToHandle(435, 400, 0xfdeff2, m_fontHandle, "\nScore:");
		DrawFormatStringToHandle(650, 400, 0x00ccff, m_fontHandle, "\n%.0f", m_finalScore);
	}
	else if (m_finalScore >= 25000)
	{
		DrawFormatStringToHandle(458, 400, 0xfdeff2, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(655, 400, 0x99ff99, m_fontHandle, "C");
		DrawFormatStringToHandle(435, 400, 0xfdeff2, m_fontHandle, "\nScore:");
		DrawFormatStringToHandle(650, 400, 0x99ff99, m_fontHandle, "\n%.0f", m_finalScore);
	}
	else
	{
		DrawFormatStringToHandle(458, 400, 0xfdeff2, m_fontHandle, "Rank:");
		DrawFormatStringToHandle(655, 400, 0xcc33ff, m_fontHandle, "D");
		DrawFormatStringToHandle(435, 400, 0xfdeff2, m_fontHandle, "\nScore:");
		DrawFormatStringToHandle(650, 400, 0xcc33ff, m_fontHandle, "\n%.0f", m_finalScore);
	}
}

void ClearScene::FadeDraw()
{
	DrawGraph(kBgDrawWidthOffset, kBgDrawHeightOffset, m_bgHandle, true);
	DrawGraph(0, 0, m_clearBgHandle, true);

	float rate = static_cast<float>(m_fadeFrameCount) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}