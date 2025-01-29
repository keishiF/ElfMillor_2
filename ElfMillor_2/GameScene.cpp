#include "GameScene.h"
#include "GameOverScene.h"
#include "ClearScene.h"
#include "SceneController.h"

#include "Player.h"
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

	// �G�̏����ʒu
	constexpr float kGroundEnemyInitPosX1 = 450.0f;
	constexpr float kGroundEnemyInitPosY1 = 7483.0f;

	constexpr float kGroundEnemyInitPosX2 = 1100.0f;
	constexpr float kGroundEnemyInitPosY2 = 6780.0f;

	constexpr float kGroundEnemyInitPosX3 = 160.0f;
	constexpr float kGroundEnemyInitPosY3 = 5435.0f;

	constexpr float kGroundEnemyInitPosX4 = 900.0f;
	constexpr float kGroundEnemyInitPosY4 = 4923.0f;

	constexpr float kGroundEnemyInitPosX5 = 250.0f;
	constexpr float kGroundEnemyInitPosY5 = 3585.0f;

	constexpr float kFlyingEnemyInitPosX1 = 320.0f;
	constexpr float kFlyingEnemyInitPosY1 = 4775.0f;

	constexpr float kFlyingEnemyInitPosX2 = 320.0f;
	constexpr float kFlyingEnemyInitPosY2 = 4275.0f;

	constexpr float kFlyingEnemyInitPosX3 = 320.0f;
	constexpr float kFlyingEnemyInitPosY3 = 3960.0f;

	constexpr float kFlyingEnemyInitPosX4 = 370.0f;
	constexpr float kFlyingEnemyInitPosY4 = 3000.0f;

	constexpr float kFlyingEnemyInitPosX5 = 630.0f;
	constexpr float kFlyingEnemyInitPosY5 = 2700.0f;

	constexpr float kFlyingEnemyInitPosX6 = 800.0f;
	constexpr float kFlyingEnemyInitPosY6 = 2600.0f;

	constexpr float kFlyingEnemyInitPosX7 = 1008.0f;
	constexpr float kFlyingEnemyInitPosY7 = 1400.0f;

	constexpr float kFlyingEnemyInitPosX8 = 728.0f;
	constexpr float kFlyingEnemyInitPosY8 = 1150.0f;
}

GameScene::GameScene(SceneController& controller) :
	SceneBase(controller),
	m_update(&GameScene::FadeInUpdate),
	m_draw(&GameScene::FadeDraw)
{
	// �t�F�[�h�p�̃J�E���g�ϐ�
	m_frame = kFadeInterval;

	// BGM�̓ǂݍ���
	m_bgmHandle = LoadSoundMem("data/sound/BGM3.mp3");
	assert(m_bgmHandle != -1);

	// �}�b�v�̏�����
	m_map = std::make_shared<Map>();
	m_map->InitMap();

	m_camera = std::make_shared<Camera>();

	// �v���C���[�̏�����
	m_player = std::make_shared<Player>(m_camera);
	m_player->Init();

	// �n��G�l�~�[�̏�����
	m_groundEnemyArray.resize(5);
	CreateGroundEnemy(kGroundEnemyInitPosX1, kGroundEnemyInitPosY1);
	CreateGroundEnemy(kGroundEnemyInitPosX2, kGroundEnemyInitPosY2);
	CreateGroundEnemy(kGroundEnemyInitPosX3, kGroundEnemyInitPosY3);
	CreateGroundEnemy(kGroundEnemyInitPosX4, kGroundEnemyInitPosY4);
	CreateGroundEnemy(kGroundEnemyInitPosX5, kGroundEnemyInitPosY5);

	// ��s�G�l�~�[�̏�����
	m_flyingEnemyArray.resize(8);
	CreateFlyingEnemy(kFlyingEnemyInitPosX1, kFlyingEnemyInitPosY1);
	CreateFlyingEnemy(kFlyingEnemyInitPosX2, kFlyingEnemyInitPosY2);
	CreateFlyingEnemy(kFlyingEnemyInitPosX3, kFlyingEnemyInitPosY3);
	CreateFlyingEnemy(kFlyingEnemyInitPosX4, kFlyingEnemyInitPosY4);
	CreateFlyingEnemy(kFlyingEnemyInitPosX5, kFlyingEnemyInitPosY5);
	CreateFlyingEnemy(kFlyingEnemyInitPosX6, kFlyingEnemyInitPosY6);
	CreateFlyingEnemy(kFlyingEnemyInitPosX7, kFlyingEnemyInitPosY7);
	CreateFlyingEnemy(kFlyingEnemyInitPosX8, kFlyingEnemyInitPosY8);

	// �J�����̏�����
	m_camera->Init(m_player);

	// BGM�̍Đ�
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
}

GameScene::~GameScene()
{
	DeleteSoundMem(m_bgmHandle);
}

void GameScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void GameScene::NormalUpdate(Input& input)
{
	m_camera->Update();
	m_player->Update(input, m_groundEnemyArray, m_flyingEnemyArray, *m_map);

	for (int i = 0; i < m_groundEnemyArray.size(); i++)
	{
		if (m_groundEnemyArray[i])
		{
			m_groundEnemyArray[i]->Update(*m_player, *m_map);
		}
	}

	for (int i = 0; i < m_flyingEnemyArray.size(); i++)
	{
		if (m_flyingEnemyArray[i])
		{
			m_flyingEnemyArray[i]->Update(*m_player, *m_map);
		}
	}

	if (m_player->m_isClearFlag)
	{
		m_update = &GameScene::FadeOutUpdate;
		m_draw = &GameScene::FadeDraw;
		m_frame = 0;
	}

	if (m_player->GetHp() <= 0)
	{
		m_player->DeadUpdate();

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
	if (m_player->m_isClearFlag)
	{
		// ����ChangeScene���Ăяo���ꂽ�����GameScene�I�u�W�F�N�g�͏��ł��Ă���
		m_controller.ChangeScene(std::make_shared<ClearScene>(m_controller));

		// ����������ł���̂ł����]�v�ȏ����������Ă���Ƃ܂����̂�return;
		return;
	}
	else
	{
		if (m_frame++ >= 60)
		{
			// ����ChangeScene���Ăяo���ꂽ�����GameScene�I�u�W�F�N�g�͏��ł��Ă���
			m_controller.ChangeScene(std::make_shared<GameOverScene>(m_controller));

			// ����������ł���̂ł����]�v�ȏ����������Ă���Ƃ܂����̂�return;
			return;
		}
		// �����ɉ����������������ꍇ�A��L��return���Ȃ����
		// �����傪����ł���̂ɉ����]���r���������邱�ƂɂȂ適�F�X�܂�����
	}
}

void GameScene::Draw()
{
	(this->*m_draw)();
}

void GameScene::NormalDraw()
{
	m_map->DrawMap(m_camera);
	m_player->Draw();

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