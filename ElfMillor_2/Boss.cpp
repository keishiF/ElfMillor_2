#include "Boss.h"
#include "DxLib.h"

#include <cassert>

namespace
{
	// �O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 224;
	constexpr int kGraphHeight = 240;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 15;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExpRate = 2.0f;

	// �����ʒu
	constexpr float kBossPosX = 700;
	constexpr float kBossPosY = 330;
}

Boss::Boss()
{
	m_animAllFrame = 0;
	m_hp = 3;
}

Boss::~Boss()
{
}

void Boss::Init()
{	
	m_handleIdle = LoadGraph("img/Boss/Boss.png");
	assert(m_handleIdle != -1);

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kIdleAnimNum);
	m_pos.x = kBossPosX;
	m_pos.y = kBossPosY;
}

void Boss::Update()
{
	m_idleAnim.Update();

	if (GetLeft() < m_bullet.GetRight())
	{
		m_hp--;
	}
}

void Boss::Draw()
{
	m_isDirLeft = false;
	m_idleAnim.Play(m_pos, m_isDirLeft);
}

float Boss::GetLeft()
{
	return m_pos.x - kGraphWidth * 0.5f;
}

float Boss::GetRight()
{
	return m_pos.x + kGraphWidth * 0.5f;;
}

float Boss::GetTop()
{
	return m_pos.y - kGraphHeight * 0.5f;;
}

float Boss::GetBottom()
{
	return m_pos.y + kGraphHeight * 0.5f;;
}