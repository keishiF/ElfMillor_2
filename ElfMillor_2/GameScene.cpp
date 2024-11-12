#include "GameScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "ResultScene.h"
#include "Player.h"

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
}

void GameScene::NormalUpdate(Input& input)
{
	Player player;
	player.Init();
}

void GameScene::FadeDraw()
{
}

void GameScene::NormalDraw()
{
}

GameScene::GameScene(SceneController& controller):
	SceneBase(controller),
	m_update(&GameScene::FadeInUpdate),
	m_draw(&GameScene::FadeDraw)
{
	m_backHandle = LoadGraph("img/BackGround/luffy.png");
	assert(m_backHandle != -1);
	m_frame = kFadeInterval;
}

void GameScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameScene::Draw()
{
	(this->*m_draw)();
}