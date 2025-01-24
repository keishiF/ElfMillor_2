#include "FlyingEnemy.h"
#include "Player.h"
#include "Camera.h"
#include "Map.h"

#include "DxLib.h"
#include <cassert>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

namespace
{
	// �O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 150;
	constexpr int kGraphHeight = 120;

	// ����HP
	constexpr int kDefaultHp = 3;

	// �d��
	constexpr float kGravity = 0.4f;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kWalkAnimNum = 8;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExtRate = 2.5f;

	// �O���t�B�b�N�̉�]��
	constexpr float kRotaRate = 0.0f;

	// �ړ����x
	constexpr float kSpeed = 2.5f;

	// �m�b�N�o�b�N����
	constexpr int kBesideHit = 43;

	// �_���[�W�H�������̖��G����
	constexpr int kDamageBlinkFrame = 30;

	// �ړ������̐���
	constexpr float kMoveDistance = 150.0f;
}

FlyingEnemy::FlyingEnemy(std::weak_ptr<Camera> camera):
	m_handleFly(-1),
	m_isDirLeft(false),
	m_blinkFrameCount(0),
	m_moveDir(1),
	m_initPosY(0.0f),
	EnemyBase(Vec3(0.0f, 0.0f), camera)
{
}

FlyingEnemy::~FlyingEnemy()
{
}

void FlyingEnemy::Init(float posX, float posY)
{
	m_handleFly = LoadGraph("img/Enemy/But/Flight.png");
	assert(m_handleFly != -1);

	m_idleAnim.Init(m_handleFly, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kWalkAnimNum);

	m_hp = kDefaultHp;

	m_pos.x = posX;
	m_pos.y = posY;

	m_initPosY = posY;
}

void FlyingEnemy::Update(Map& map)
{
	UpdateBlinkFrame();

	m_idleAnim.Update();

	// �ړ�����
	m_pos.y += kSpeed * m_moveDir;

	// ��苗���ړ�����������𔽓]
	if (std::abs(m_pos.y - m_initPosY) >= kMoveDistance)
	{
		m_moveDir *= -1;
	}
}

void FlyingEnemy::Draw()
{
	// �_�ŏ���
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	Vec3 camOffset = m_camera.lock()->GetDrawOffset();
	camOffset.x = 0;

#ifdef DISP_COLLISION
	if (m_hp > 0)
	{
		DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop() + camOffset.y),
			static_cast<int>(GetRight()), static_cast<int>(GetBottom() + camOffset.y), 0xff0000, false);
	}
#endif

	m_idleAnim.Play(m_pos + camOffset, m_isDirLeft);
}

void FlyingEnemy::End()
{
	DeleteGraph(m_handleFly);
}

float FlyingEnemy::GetLeft()
{
	return (m_pos.x - 40);
}

float FlyingEnemy::GetRight()
{
	return (m_pos.x + 50);
}

float FlyingEnemy::GetTop()
{
	return (m_pos.y);
}

float FlyingEnemy::GetBottom()
{
	return (m_pos.y + 80);
}

Rect FlyingEnemy::GetRect()
{
	// ��`�����蔻����쐬
	Rect rect;
	rect.top = GetTop();
	rect.bottom = GetBottom();
	rect.right = GetRight();
	rect.left = GetLeft();
	return rect;
}

void FlyingEnemy::OnDamage()
{
	// ���Ƀ_���[�W���󂯂Ă���(���G���Ԃ�)
	// �ēx�_���[�W���󂯂邱�Ƃ͖���
	if (m_blinkFrameCount > 0) return;

	// ���G����(�_�ł��鎞��)��ݒ肷��
	m_blinkFrameCount = kDamageBlinkFrame;

	// �_���[�W���󂯂�
	m_hp--;
}

void FlyingEnemy::UpdateBlinkFrame()
{
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}
}