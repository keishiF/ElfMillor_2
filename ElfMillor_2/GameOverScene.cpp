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

	// ボタンの座標
	constexpr int kTitleButtonPosX = 360;
	constexpr int kTitleButtonPosY = 360;
}

GameOverScene::GameOverScene(SceneController& controller) :
	SceneBase(controller),
	m_blinkFrameCount(0),
	m_update(&GameOverScene::FadeInUpdate),
	m_draw(&GameOverScene::FadeDraw),
	m_bgHandle(-1),
	m_fontHandle(-1),
	m_seHandle(-1),
	m_bgmHandle(-1)
{
	m_fadeFrameCount = kFadeInterval;
	m_bgHandle = LoadGraph("data/image/BackGround/GameOver.png");

	m_fontHandle = CreateFontToHandle("Algerian", 48, -1, DX_FONTTYPE_ANTIALIASING_8X8);
	assert(m_fontHandle != -1);

	m_seHandle = LoadSoundMem("data/sound/SE/GameOverButtonSE.mp3");
	assert(m_seHandle != -1);

	// BGMの読み込み
	m_bgmHandle = LoadSoundMem("data/sound/BGM/GameOverBGM.mp3");
	assert(m_bgmHandle != -1);

	// BGMの再生
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
}

GameOverScene::~GameOverScene()
{
	DeleteGraph(m_bgHandle);
	DeleteFontToHandle(m_fontHandle);
	DeleteSoundMem(m_seHandle);
	DeleteSoundMem(m_bgmHandle);
}

void GameOverScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameOverScene::NormalUpdate(Input& input)
{
	++m_blinkFrameCount;

	if (input.IsPress(PAD_INPUT_3))
	{
		PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK, true);
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
	DrawGraph(0, 0, m_bgHandle, true);

	// 点滅効果のための条件
	if ((m_blinkFrameCount / 30) % 2 == 0)
	{
		// 文字を描画
		DrawStringToHandle(kTitleButtonPosX, kTitleButtonPosY, "PRESS ANY BUTTON\n　　　　　　　　　RESTART", 0xa0d8ef, m_fontHandle);
	}
}

void GameOverScene::FadeDraw()
{
	DrawGraph(0, 0, m_bgHandle, true);

	float rate = static_cast<float>(m_fadeFrameCount) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}