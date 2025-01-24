#include "GameScene.h"
#include "GameOverScene.h"
#include "SceneController.h"

#include "Player.h"
#include "Boss.h"
#include "GroundEnemy.h"
#include "FlyingEnemy.h"
#include "Camera.h"
#include "Map.h"

#include "game.h"
#include "Input.h"

#include "DxLib.h"
#include <memory>
#include <cassert>

namespace
{
	constexpr int kFadeInterval = 60;

	// 敵の初期位置
	constexpr float kGroundEnemyInitPosX1 = 450.0f;
	constexpr float kGroundEnemyInitPosY1 = 6044.0f;

	constexpr float kGroundEnemyInitPosX2 = 1100.0f;
	constexpr float kGroundEnemyInitPosY2 = 5340.0f;

	constexpr float kGroundEnemyInitPosX3 = 160.0f;
	constexpr float kGroundEnemyInitPosY3 = 3997.0f;

	constexpr float kGroundEnemyInitPosX4 = 900.0f;
	constexpr float kGroundEnemyInitPosY4 = 3484.0f;

	constexpr float kGroundEnemyInitPosX5 = 250.0f;
	constexpr float kGroundEnemyInitPosY5 = 2140.0f;

	constexpr float kFlyingEnemyInitPosX1 = 320.0f;
	constexpr float kFlyingEnemyInitPosY1 = 4800.0f;
}

GameScene::GameScene(SceneController& controller) :
	SceneBase(controller),
	m_update(&GameScene::FadeInUpdate),
	m_draw(&GameScene::FadeDraw)
{
	m_frame = kFadeInterval;

	m_map = std::make_shared<Map>();
	m_map->InitMap();

	m_camera = std::make_shared<Camera>();

	m_boss = std::make_shared<Boss>(m_camera);
	m_boss->Init(0.0f, 0.0f);

	m_player = std::make_shared<Player>(m_camera);
	m_player->Init();

	m_groundEnemyArray.resize(5);
	CreateGroundEnemy(kGroundEnemyInitPosX1, kGroundEnemyInitPosY1);
	CreateGroundEnemy(kGroundEnemyInitPosX2, kGroundEnemyInitPosY2);
	CreateGroundEnemy(kGroundEnemyInitPosX3, kGroundEnemyInitPosY3);
	CreateGroundEnemy(kGroundEnemyInitPosX4, kGroundEnemyInitPosY4);
	CreateGroundEnemy(kGroundEnemyInitPosX5, kGroundEnemyInitPosY5);

	m_flyingEnemyArray.resize(1);
	CreateFlyingEnemy(kFlyingEnemyInitPosX1, kFlyingEnemyInitPosY1);

	m_camera->Init(m_player);
}

void GameScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameScene::NormalUpdate(Input& input)
{
	m_camera->Update();
	m_player->Update(input, *m_boss, m_groundEnemyArray, *m_map);
	//m_boss->Update();

	for (int i = 0; i < m_groundEnemyArray.size(); i++)
	{
		if (m_groundEnemyArray[i])
		{
			m_groundEnemyArray[i]->Update(*m_map);
		}
	}

	for (int i = 0; i < m_flyingEnemyArray.size(); i++)
	{
		if (m_flyingEnemyArray[i])
		{
			m_flyingEnemyArray[i]->Update(*m_map);
		}
	}

	float disX = m_boss->m_pos.x - m_player->m_pos.x;
	if (disX > 50)
	{
		m_boss->Shot();
	}

	if (m_player->GetHp() <= 0)
	{
		m_update = &GameScene::FadeOutUpdate;
		m_draw = &GameScene::FadeDraw;
		m_frame = 0;
	}
}

void GameScene::FadeInUpdate(Input& input)
{
	if (m_frame-- <= 0)
	{
		m_update = &GameScene::NormalUpdate;
		m_draw   = &GameScene::NormalDraw;
	}
}

void GameScene::FadeOutUpdate(Input& input)
{
	if (m_frame++ >= 60)
	{
		// このChangeSceneが呼び出された直後はGameSceneオブジェクトは消滅している
		m_controller.ChangeScene(std::make_shared<GameOverScene>(m_controller));

		// 自分が死んでいるのでもし余計な処理が入っているとまずいのでreturn;
		return;
	}
	// ここに何か処理があった場合、上記のreturnがなければ
	// 持ち主が死んでいるのに何かゾンビ処理をすることになる←色々まっずい
}

void GameScene::Draw()
{
	(this->*m_draw)();
}

void GameScene::NormalDraw()
{
	m_map->DrawMap(m_camera);
	m_player->Draw();
	//m_boss->Draw();

	for (int i = 0; i < m_groundEnemyArray.size(); i++)
	{
		if (m_groundEnemyArray[i])
		{
			m_groundEnemyArray[i]->Draw();
		}
	}

	for (int i = 0; i < m_flyingEnemyArray.size(); i++)
	{
		if (m_flyingEnemyArray[i])
		{
			m_flyingEnemyArray[i]->Draw();
		}
	}
}

void GameScene::FadeDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::CreateGroundEnemy(float posX, float posY)
{
	for (int i = 0; i < m_groundEnemyArray.size(); i++)
	{
		if (!m_groundEnemyArray[i])
		{
			m_groundEnemyArray[i] = std::make_shared<GroundEnemy>(m_camera);
			m_groundEnemyArray[i]->Init(posX, posY);
			return;
		}
	}
}

void GameScene::CreateFlyingEnemy(float posX, float posY)
{

	for (int i = 0; i < m_flyingEnemyArray.size(); i++)
	{
		if (!m_flyingEnemyArray[i])
		{
			m_flyingEnemyArray[i] = std::make_shared<FlyingEnemy>(m_camera);
			m_flyingEnemyArray[i]->Init(posX, posY);
			return;
		}
	}
}