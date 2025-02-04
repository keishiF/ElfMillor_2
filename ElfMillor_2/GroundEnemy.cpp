#include "GroundEnemy.h"
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
	constexpr int kGraphWidth  = 100;
	constexpr int kGraphHeight = 100;

	// ���S���̃G�t�F�N�g�̃O���t�B�b�N�T�C�Y
	constexpr int kEffectGraphWidth = 48;
	constexpr int kEffectGraphHeight = 48;

	// ��ʒ[
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	// ����HP
	constexpr int kDefaultHp = 3;

	// �d��
	constexpr float kGravity = 0.4f;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kWalkAnimNum = 8;
	constexpr int kDeadAnimNum = 18;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;
	constexpr int kDeadAnimSingleFrame = 3;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExtRate = 3.5f;
	constexpr float kEffectExtRate = 3.75f;

	// �O���t�B�b�N�̉�]��
	constexpr float kRotaRate = 0.0f;

	// �ړ����x
	constexpr float kSpeed = 2.5f;

	// �m�b�N�o�b�N����
	constexpr int kBesideHit   = 43;

	// �_���[�W�H�������̖��G����
	constexpr int kDamageBlinkFrame = 30;

	// �X�R�A
	constexpr int kAddScore = 1500;
}

GroundEnemy::GroundEnemy(std::weak_ptr<Camera> camera) :
	m_handleRun(-1),
	m_handleDead(-1),
	m_seHandle(-1),
	m_isDirLeft(false),
	m_isDead(false),
	m_blinkFrameCount(0),
	m_deadAnim(),
	EnemyBase(Vec3(0.0f, 0.0f), camera)
{
}

GroundEnemy::~GroundEnemy()
{
}

void GroundEnemy::Init(float posX, float posY)
{
	m_handleRun = LoadGraph("data/image/Enemy/Orc/OrcWalk.png");
	assert(m_handleRun != -1);

	m_handleDead = LoadGraph("data/image/Effect/effect2.png");
	assert(m_handleDead != -1);

	m_seHandle = LoadSoundMem("data/sound/SE/deadSE.mp3");
	assert(m_seHandle != -1);

	m_runAnim.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kWalkAnimNum);
	m_deadAnim.Init(m_handleDead, kDeadAnimSingleFrame, kEffectGraphWidth, kEffectGraphHeight, kEffectExtRate, kRotaRate, kDeadAnimNum);

	m_hp = kDefaultHp;

	m_pos.x = posX;
	m_pos.y = posY;
}

void GroundEnemy::Update(Player& player, Map& map)
{
	UpdateBlinkFrame();

	if (m_isDead)
	{
		m_deadAnim.Update();
		PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK, true);
		if (m_deadAnim.IsEnd())
		{
			DeleteGraph(m_handleRun);
			DeleteGraph(m_handleDead);
			DeleteGraph(m_seHandle);
		}
	}

	if (!m_isDead)
	{
		m_runAnim.Update();

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
				// �E�ǂɓ������Ă���̂ō������ɂ���
				m_isDirLeft = true;
				m_pos.x = chipRect.left - kBesideHit;
			}
			// �������Ɉړ����Ă���
			else if (m_vec.x < 0.0f)
			{
				// ���ǂɓ������Ă���̂ŉE�����ɂ���
				m_isDirLeft = false;
				m_pos.x = chipRect.right + kBesideHit + 30;
			}
		}

		// �v���C���[�Ɠ����������̏���
		if (GetLeft() < player.GetRight() &&
			GetRight() > player.GetLeft() &&
			GetTop() < player.GetBottom() &&
			GetBottom() > player.GetTop())
		{
			// �v���C���[�Ƀ_���[�W
			player.OnDamage();
			// �����������Ƀv���C���[�̔��Α��Ɍ���
			if (player.GetPos().x < m_pos.x)
			{
				if (m_isDirLeft)
				{
					m_isDirLeft = false;
				}
			}
			else if (player.GetPos().x > m_pos.x)
			{
				if (!m_isDirLeft)
				{
					m_isDirLeft = true;
				}
			}
		}

		// ���[�ɍs������E�����ɂ���
		if (m_pos.x <= kLeftEndWidth)
		{
			m_isDirLeft = false;
		}
		// �E�[�ɍs�����獶�����ɂ���
		else if (m_pos.x >= kRightEndWidth)
		{
			m_isDirLeft = true;
		}
	}

	// ���S
	if (m_hp <= 0 && !m_isDead)
	{
		m_isDead = true;
		player.GetScoreManager().AddScore(kAddScore);
	}
}

void GroundEnemy::Draw()
{
	// �_�ŏ���
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	//Vec3 drawPos = m_camera.Capture(m_pos);

	Vec3 camOffset = m_camera.lock()->GetDrawOffset();
	camOffset.x = 0;

#ifdef DISP_COLLISION
	if (m_hp > 0)
	{
		DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop() + camOffset.y),
			static_cast<int>(GetRight()), static_cast<int>(GetBottom() + camOffset.y), 0xff0000, false);
	}
#endif

	if (m_hp <= 0)
	{
		m_deadAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
	else
	{
		m_runAnim.Play(m_pos + camOffset, m_isDirLeft);
	}

	/*DrawFormatString(0, 30, 0xffffff, "EnemyPos.X~%f, Y=%f", m_pos.x, m_pos.y);
	DrawFormatString(0, 45, 0xffffff, "drawPos.X~%f, Y=%f", drawPos.x, drawPos.y);*/
}

float GroundEnemy::GetLeft()
{
	return (m_pos.x - 20);
}

float GroundEnemy::GetRight()
{
	return (m_pos.x + 25);
}

float GroundEnemy::GetTop()
{
	return (m_pos.y - 35);
}

float GroundEnemy::GetBottom()
{
	return (m_pos.y + 25);
}

Rect GroundEnemy::GetRect()
{
	// ��`�����蔻����쐬
	Rect rect;
	rect.top    = GetTop();
	rect.bottom = GetBottom();
	rect.right  = GetRight();
	rect.left   = GetLeft();
	return rect;
}

void GroundEnemy::OnDamage()
{
	// ���Ƀ_���[�W���󂯂Ă���(���G���Ԃ�)
	// �ēx�_���[�W���󂯂邱�Ƃ͖���
	if (m_blinkFrameCount > 0) return;

	// ���G����(�_�ł��鎞��)��ݒ肷��
	m_blinkFrameCount = kDamageBlinkFrame;

	// �_���[�W���󂯂�
	m_hp--;
}

void GroundEnemy::UpdateBlinkFrame()
{
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}
}