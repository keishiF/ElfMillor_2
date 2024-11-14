#include "GameScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "ResultScene.h"
#include "Player.h"
#include "Bullet.h"

#include <memory>
#include <cassert>

namespace
{
	constexpr int kFadeInterval = 60;
}

void GameScene::FadeInUpdate(Input& input)
{
	if (m_frame-- <= 0)
	{
		m_update = &GameScene::NormalUpdate;
		m_draw = &GameScene::NormalDraw;
	}
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (m_frame++ >= 60)
	{
		// このChangeSceneが呼び出された直後はGameSceneオブジェクトは消滅している
		m_controller.ChangeScene(std::make_shared<ResultScene>(m_controller));

		// 自分が死んでいるのでもし余計な処理が入っているとまずいのでreturn;
		return;
	}
	// ここに何か処理があった場合、上記のreturnがなければ
	// 持ち主が死んでいるのに何かゾンビ処理をすることになる←色々まっずい
}

void GameScene::NormalUpdate(Input& input)
{
	m_player->Update(input);
	if (input.IsPress(PAD_INPUT_3))
	{
		m_update = &GameScene::FadeOutUpdate;
		m_draw = &GameScene::FadeDraw;
		m_frame = 1;
	}
}

void GameScene::FadeDraw()
{
	DrawBox(0, 0, 160, 720, 0xffffff, true);
	DrawBox(1120, 0, 1280, 720, 0xffffff, true);
	DrawString(600, 100, "GameScene", 0xffffff);
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::NormalDraw()
{
	m_player->Draw();
	DrawString(600, 100, "GameScene", 0xffffff);
	DrawBox(0, 0, 160, 720, 0xffffff, true);
	DrawBox(1120, 0, 1280, 720, 0xffffff, true);
	DrawLine(160, 500, 1120, 500, 0xffffff, false);
}

GameScene::GameScene(SceneController& controller):
	SceneBase(controller),
	m_update(&GameScene::FadeInUpdate),
	m_draw(&GameScene::FadeDraw)
{
	m_backHandle = LoadGraph("img/BackGround/luffy.png");
	assert(m_backHandle != -1);
	m_frame = kFadeInterval;

	m_player = std::make_shared<Player>();
	m_player->Init();

	//m_bullet = std::make_shared<Bullet>();
	//m_bullet->Init();
}

void GameScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameScene::Draw()
{
	(this->*m_draw)();
}