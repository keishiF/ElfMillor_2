#include "Enemy1.h"
#include "DxLib.h"
#include <cassert>
#include "Player.h"
#include "Camera.h"

namespace
{
	// �O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 100;
	constexpr int kGraphHeight = 100;

	// �����ʒu
	constexpr float kEnemyDefaultPosX = 420;
	constexpr float kEnemyDefaultPosY = 50;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kWalkAnimNum = 8;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExpRate = 3.5f;

	// �ړ����x
	constexpr float kSpeed = 1.0f;

	// �m�b�N�o�b�N����
	constexpr int kKnockBack = 30;
}

Enemy1::Enemy1(Camera& camera) :
	m_handleRun(-1),
	EnemyBase(Vec3(kEnemyDefaultPosX, kEnemyDefaultPosY), camera)
{
	m_animAllFrame = 0;
}

Enemy1::~Enemy1()
{
}

void Enemy1::Init()
{
	m_handleRun = LoadGraph("img/Enemy/Orc/OrcWalk.png");
	assert(m_handleRun != -1);

	m_idleRun.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kWalkAnimNum);

	m_pos.x = kEnemyDefaultPosX;
	m_pos.y = kEnemyDefaultPosY;

	m_hp = 3;
}

void Enemy1::Update()
{
}

void Enemy1::Update(Player& player)
{
	m_idleRun.Update();

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
		DeleteGraph(m_handleRun);
	}
}

void Enemy1::Draw()
{
	Vec3 drawPos = m_camera.Capture(m_pos);

	if (m_hp > 0)
	{
		DrawBox(drawPos.x - kGraphWidth * 0.5f, drawPos.y - kGraphHeight * 0.5f, drawPos.x + kGraphWidth * 0.5f, drawPos.y + kGraphHeight * 0.5f, 0xff0000, false);
	}
	m_idleRun.Play(drawPos, m_isDirLeft);

	DrawFormatString(0, 30, 0xffffff, "EnemyPos.X~%f, Y=%f", m_pos.x, m_pos.y);
	DrawFormatString(0, 45, 0xffffff, "drawPos.X~%f, Y=%f", drawPos.x, drawPos.y);
}

void Enemy1::End()
{
	DeleteGraph(m_handleRun);
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
	return (m_pos.y - kGraphHeight * 0.5f);
}

float Enemy1::GetBottom()
{
	return (m_pos.y + kGraphHeight * 0.5f);
}