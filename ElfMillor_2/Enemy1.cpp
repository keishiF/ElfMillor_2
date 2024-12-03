#include "Enemy1.h"
#include "DxLib.h"
#include <cassert>
#include "Player.h"

namespace
{
	// �O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 100;
	constexpr int kGraphHeight = 100;

	// �����ʒu
	constexpr float kEnemyDefaultPosX = 900;
	constexpr float kEnemyDefaultPosY = 530;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 6;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExpRate = 3.5f;

	// �ړ����x
	constexpr float kSpeed = 2.0f;
}

Enemy1::Enemy1():
	m_handleIdle(-1)
{
	m_animAllFrame = 0;
}

Enemy1::~Enemy1()
{
}

void Enemy1::Init()
{
	m_handleIdle = LoadGraph("img/Enemy/Orc/OrcIdle.png");
	assert(m_handleIdle != -1);

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kIdleAnimNum);

	m_pos.x = kEnemyDefaultPosX;
	m_pos.y = kEnemyDefaultPosY;

	m_hp = 3;
}

void Enemy1::Update()
{
}

void Enemy1::Update(Player& player)
{
	m_idleAnim.Update();

	if (m_pos.x > player.m_pos.x)
	{
		m_vec.x -= kSpeed;
		m_isDirLeft = true;
	}
	else if (m_pos.x < player.m_pos.x)
	{
		m_vec.x += kSpeed;
		m_isDirLeft = false;
	}

	// �ړ�����
	m_vec = m_vec.GetNormalize() * kSpeed;
	m_pos += m_vec;

	// ���S
	if (m_hp <= 0)
	{
		DeleteGraph(m_handleIdle);
	}
}

void Enemy1::Draw()
{
	DrawBox(m_pos.x - kGraphWidth * 0.5f, m_pos.y - kGraphHeight * 0.5f, m_pos.x + kGraphWidth * 0.5f, m_pos.y + kGraphHeight * 0.5f, 0xff0000, false);
	m_idleAnim.Play(m_pos, m_isDirLeft);
}

void Enemy1::End()
{
	DeleteGraph(m_handleIdle);
}

float Enemy1::GetLeft()
{
	return (m_pos.x - kGraphWidth * 0.5f);
}

float Enemy1::GetRight()
{
	return (m_pos.x + kGraphWidth * 0.5f);
}

float Enemy1::GetTop()
{
	return (m_pos.y - kGraphHeight);
}

float Enemy1::GetBottom()
{
	return (m_pos.y + kGraphHeight * 0.5f);
}