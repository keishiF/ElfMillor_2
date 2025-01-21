#include "Boss.h"

#include "DxLib.h"
#include <cassert>

namespace
{
	// �O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth  = 224;
	constexpr int kGraphHeight = 240;

	// �e�̃O���t�B�b�N�T�C�Y
	constexpr int kShotGraphWidth  = 64;
	constexpr int kShotGraphHeight = 64;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 15;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExtRate  = 2.0f;

	// �O���t�B�b�N�̉�]��
	constexpr float kRotaRate = 0.0f;

	// �����ʒu
	constexpr float kBossPosX = 700;
	constexpr float kBossPosY = -330;
}

Boss::Boss(std::weak_ptr<Camera> camera) :
	m_handleIdle(-1),
	m_handleShot(-1),
	m_isDirLeft(false),
	EnemyBase(Vec3(kBossPosX, kBossPosY), camera)
{
}

Boss::~Boss()
{
}

void Boss::Init(float posX, float posY)
{	
	m_handleIdle = LoadGraph("img/Boss/Boss.png");
	assert(m_handleIdle != -1);

	m_handleShot = LoadGraph("img/Boss/BossBullet.png");
	assert(m_handleShot != -1);

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kIdleAnimNum);

	m_pos.x = kBossPosX;
	m_pos.y = kBossPosY;

	m_hp = 3;
}

void Boss::Update(Map& map)
{
	m_idleAnim.Update();

	if (m_hp <= 0)
	{
		End();
	}
}

void Boss::Draw()
{
	m_isDirLeft = false;
	m_idleAnim.Play(m_pos, m_isDirLeft);
}

void Boss::End()
{
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleShot);
}

void Boss::Shot()
{
	if (m_hp > 0)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 100, 0xffffff, false);
	}
	DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		0, 0, kShotGraphWidth, kGraphHeight, 1.5f, 0.0f, m_handleShot, true);
}

float Boss::GetLeft()
{
	return (m_pos.x - kGraphWidth * 0.5f);
}

float Boss::GetRight()
{
	return (m_pos.x + kGraphWidth * 0.5f);
}

float Boss::GetTop()
{
	return (m_pos.y - kGraphHeight);
}

float Boss::GetBottom()
{
	return (m_pos.y + kGraphHeight * 0.5f);
}