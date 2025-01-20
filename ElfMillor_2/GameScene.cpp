#include "GameScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "GameOverScene.h"
#include "Player.h"
#include "Boss.h"
#include "Enemy1.h"
#include "Map.h"
#include "game.h"
#include "Camera.h"
#include "Vec3.h"

#include <memory>
#include <cassert>

namespace
{
	constexpr int kFadeInterval = 60;

	// 敵の初期位置
	constexpr float kEnemyDefaultPosX = 450;
	constexpr float kEnemyDefaultPosY = 4443;

	constexpr float kEnemyDefaultPosX2 = 1100;
	constexpr float kEnemyDefaultPosY2 = 3740;
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

void GameScene::NormalUpdate(Input& input)
{
	m_camera->Update();
	m_player->Update(input, *m_boss, *m_enemy1, *m_map, *m_camera);
	//m_boss->Update();
	m_enemy1->Update(*m_map);

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

void GameScene::FadeDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::NormalDraw()
{
	m_map->DrawMap(*m_camera);
	m_player->Draw(*m_camera);
	//m_boss->Draw();
	m_enemy1->Draw(*m_camera);
}

void GameScene::CreateEnemy(float posX, float posY)
{
	/*for (int i = 0; i < m_enemy.size(); i++)
	{
		m_enemy[i].Init(posX, posY);
		return;
	}*/
}

GameScene::GameScene(SceneController& controller):
	SceneBase(controller),
	m_update(&GameScene::FadeInUpdate),
	m_draw(&GameScene::FadeDraw)
{
	m_frame  = kFadeInterval;

	m_map    = std::make_shared<Map>();
	m_map->InitMap();

	m_camera = std::make_shared<Camera>();

	m_boss   = std::make_shared<Boss>(*m_camera);
	m_boss->Init(0.0f, 0.0f);

	m_enemy1 = std::make_shared<Enemy1>(*m_camera);
	m_enemy1->Init(kEnemyDefaultPosX, kEnemyDefaultPosY);

	m_player = std::make_shared<Player>(*m_camera);
	m_player->Init();

	/*m_enemy.resize(10);
	CreateEnemy(kEnemyDefaultPosX, kEnemyDefaultPosY);
	CreateEnemy(kEnemyDefaultPosX, kEnemyDefaultPosY2);*/

	m_camera->Init(m_player);
}

void GameScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameScene::Draw()
{
	(this->*m_draw)();
}