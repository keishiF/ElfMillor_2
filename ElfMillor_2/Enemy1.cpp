#include "Enemy1.h"
#include "DxLib.h"
#include <cassert>
#include "Player.h"
#include "Camera.h"
#include "Map.h"

#ifdef _DEBUG
#define DISP_COLLISION
#endif

namespace
{
	// �O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth  = 100;
	constexpr int kGraphHeight = 100;

	// �����ʒu
	constexpr float kEnemyDefaultPosX = 500;
	constexpr float kEnemyDefaultPosY = 4443;

	// �d��
	constexpr float kGravity = 0.4f;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kWalkAnimNum = 8;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExpRate = 3.5f;

	// �ړ����x
	constexpr float kSpeed = 2.5f;

	// �m�b�N�o�b�N����
	constexpr int kBesideHit   = 43;

	// �_���[�W�H�������̖��G����
	constexpr int kDamageBlinkFrame = 30;
}

Enemy1::Enemy1(Camera& camera) :
	m_handleRun(-1),
	m_isDirLeft(false),
	m_blinkFrameCount(0),
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

void Enemy1::Draw()
{
}

void Enemy1::Update(Map& map)
{
	m_idleRun.Update();

	// �ړ�����
	if (!m_isDirLeft)
	{
		m_vec.x = kSpeed;
	}
	else if (m_isDirLeft)
	{
		m_vec.x = -kSpeed;
	}

	m_pos.x += m_vec.x;
	// �����瓖�����Ă��邩�ǂ������m�F����
	Rect chipRect;
	if (map.IsCol(GetRect(), chipRect, m_camera))
	{
		// ���E�ǂ������瓖��������

		// �E�����Ɉړ����Ă���
		if (m_vec.x > 0.0f)
		{
			// �E�ǂɓ������Ă���̂ō��ɉ����߂�
			m_isDirLeft = true;
			m_pos.x = chipRect.left - kBesideHit;
		}
		// �������Ɉړ����Ă���
		else if (m_vec.x < 0.0f)
		{
			// ���ǂɓ������Ă���̂ŉE�ɉ����߂�
			m_isDirLeft = false;
			m_pos.x = chipRect.right + kBesideHit + 30;
		}
	}

	// ���S
	if (m_hp <= 0)
	{
		DeleteGraph(m_handleRun);
	}
}

void Enemy1::Draw(Camera& camera)
{
	// �_�ŏ���
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	//Vec3 drawPos = m_camera.Capture(m_pos);

	Vec3 camOffset = camera.GetDrawOffset();
	camOffset.x = 0;

#ifdef DISP_COLLISION
	if (m_hp > 0)
	{
		DrawBox(GetLeft(), GetTop() + camOffset.y, GetRight(), GetBottom() + camOffset.y, 0xff0000, false);
	}
	m_idleRun.Play(m_pos + camOffset, m_isDirLeft);
#endif

	/*DrawFormatString(0, 30, 0xffffff, "EnemyPos.X~%f, Y=%f", m_pos.x, m_pos.y);
	DrawFormatString(0, 45, 0xffffff, "drawPos.X~%f, Y=%f", drawPos.x, drawPos.y);*/
}

void Enemy1::End()
{
	DeleteGraph(m_handleRun);
}

float Enemy1::GetLeft()
{
	return (m_pos.x - 20);
}

float Enemy1::GetRight()
{
	return (m_pos.x + 25);
}

float Enemy1::GetTop()
{
	return (m_pos.y - 35);
}

float Enemy1::GetBottom()
{
	return (m_pos.y + 35);
}

Rect Enemy1::GetRect()
{
	// �v���C���[�̋�`�����蔻����쐬
	Rect rect;
	rect.top    = GetTop();
	rect.bottom = GetBottom();
	rect.right  = GetRight();
	rect.left   = GetLeft();
	return rect;
}

void Enemy1::OnDamage()
{
	// ���Ƀ_���[�W���󂯂Ă���(���G���Ԃ�)
	// �ēx�_���[�W���󂯂邱�Ƃ͖���
	if (m_blinkFrameCount > 0) return;

	// ���G����(�_�ł��鎞��)��ݒ肷��
	m_blinkFrameCount = kDamageBlinkFrame;

	// �_���[�W���󂯂�
	m_hp--;
}
