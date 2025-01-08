#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"
#include "Boss.h"
#include "Enemy1.h"
#include "Map.h"
#include "Camera.h"

#include <cassert>

namespace
{
	// �����ʒu
	constexpr int kDefaultPlayerPosX = 360;
	constexpr int kDefaultPlayerPosY = 500;

	// ��ʒ[
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	constexpr int kFieldHeight = 352;

	// �v���C���[�̏���HP
	constexpr int kDefaultHp = 15;

	// �v���C���[�̈ړ����x
	constexpr float kSpeed = 5.0f;

	// �d��
	constexpr float kGravity = 0.4f;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �L�����N�^�[�̃O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 160;
	constexpr int kGraphHeight = 128;

	// �����蔻��̃T�C�Y
	constexpr int kColSizeWidth = 0;
	constexpr int kColSizeHeight = 0;

	// �ҋ@�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 8;
	// ����A�j���[�V�����̃R�}��
	constexpr int kRunAnimNum = 8;
	// �U���A�j���[�V�����̃R�}��
	constexpr int kAtkAnimNum = 13;
	// ���S�A�j���[�V�����̃R�}��
	constexpr int kDeathAnimNum = 10;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExpRate = 1.75f;

	// �m�b�N�o�b�N����
	constexpr int kBesideHit = 43;
	constexpr int kVerticalHit = 15;
}

Player::Player(Camera& camera) :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleAtk(-1),
	m_handleDeath(-1),
	m_isRun(false),
	m_isJump(false),
	m_isAtk(false),
	m_isDeath(false),
	m_jumpSpeed(-11.0f),
	m_jumpCount(0),
	m_vec(0.0f,0.0f),
	m_isDirLeft(false),
	m_isShotDirRight(true),
	m_blinkFrame(0),
	m_hp(kDefaultHp),
	m_isGroundHit(false),
	m_isCeilingHit(false),
	m_isLastJump(false),
	m_isLastJumpButton(false),
	m_shot(),
	m_idleAnim(),
	m_runAnim(),
	m_atkAnim(),
	// ���N���X�̏�����
	GameObject(Vec3(kDefaultPlayerPosX, kDefaultPlayerPosY), camera)
{
}

Player::~Player()
{
	// �O���t�B�b�N�̉��
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleAtk);
	DeleteGraph(m_handleDeath);
}

void Player::Init()
{
	// �O���t�B�b�N�̓ǂݍ���
	m_handleIdle = LoadGraph("img/Player2/Idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("img/Player2/Run.png");
	assert(m_handleRun != -1);

	m_handleAtk = LoadGraph("img/Player2/Atk.png");
	assert(m_handleAtk != -1);

	m_handleDeath = LoadGraph("img/Player2/Death.png");
	assert(m_handleDeath != -1);

	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Init();
	}

	m_hp = kDefaultHp;

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kIdleAnimNum);
	m_runAnim.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kRunAnimNum);
	m_atkAnim.Init(m_handleAtk, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kAtkAnimNum);
}

void Player::End()
{	
}

void Player::Update(Input& input, Boss& boss, Enemy1& enemy1, Map& map)
{
	m_idleAnim.Update();

	m_isRun = false;
	m_vec.x = 0.0f;

	if (m_isRun)
	{
		m_runAnim.Update();
	}
	if (m_isAtk)
	{
		m_atkAnim.Update();
	}

	// ������
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_vec.x -= kSpeed;
		m_isShotDirRight = false;
	}
	// �E����
	else if (input.IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_vec.x += kSpeed;
		m_isShotDirRight = true;
	}

	// �W�����v
	if (m_isJump)
	{
		// �󒆂ɂ���Ƃ��̏���

		// ���t���[���������ɏd�͂�������
		m_vec.y += kGravity;

		m_pos.x += m_vec.x;

		// �����瓖���������ǂ������m�F����
		Rect chipRect;
		if (map.IsCol(GetRect(), chipRect, m_camera))
		{
			// ���E�ǂ������瓖��������

			// �v���C���[���E�����Ɉړ����Ă���
			if (m_vec.x > 0.0f)
			{
				// �E�ǂɓ������Ă���̂ō��ɉ����߂�
				m_pos.x = chipRect.left - kBesideHit;
			}
			// �v���C���[���������Ɉړ����Ă���
			else if (m_vec.x < 0.0f)
			{
				// ���ǂɓ������Ă���̂ŉE�ɉ����߂�
				m_pos.x = chipRect.right + kBesideHit;
			}
		}

		m_pos.y += m_vec.y;
		// �c���瓖���������ǂ������m�F����
		if (map.IsCol(GetRect(), chipRect, m_camera))
		{
			// �㉺�ǂ������瓖��������

			// �v���C���[���������Ɉړ����Ă���
			if (m_vec.y > 0.0f)
			{
				// ���ɓ������Ă���̂ŏ�ɉ����߂�
				m_pos.y -= m_vec.y;
				m_vec.y = 0.0f;
				m_isJump = false;
			}
			// �v���C���[��������Ɉړ����Ă���
			else if (m_vec.y < 0.0f)
			{
				// �V��ɓ������Ă���̂ŉ��ɉ����߂�
				m_pos.y = chipRect.bottom + kVerticalHit;
				m_vec.y *= -1.0f;
			}
		}
	}
	else
	{
		// �W�����v
		if (input.IsTrigger(PAD_INPUT_1) && !m_isJump)
		{
			m_isJump = true;
			m_jumpCount++;
		}
		else
		{
			m_jumpCount = 0;
		}

		if (m_isJump && m_jumpCount == 1)
		{
			m_vec.y = m_jumpSpeed;
		}
		else
		{
			m_isJump = false;
		}

		// �ړ�����
		m_pos.x += m_vec.x;

		// �����瓖�����Ă��邩�ǂ������m�F����
		Rect chipRect;
		if (map.IsCol(GetRect(), chipRect, m_camera))
		{
			// ���E�ǂ������瓖��������

			// �v���C���[���E�����Ɉړ����Ă���
			if (m_vec.x > 0.0f)
			{
				// �E�ǂɓ������Ă���̂ō��ɉ����߂�
				m_pos.x = chipRect.left - kBesideHit;
			}
			// �v���C���[���������Ɉړ����Ă���
			else if (m_vec.x < 0.0f)
			{
				// ���ǂɓ������Ă���̂ŉE�ɉ����߂�
				m_pos.x = chipRect.right + kBesideHit;
			}
		}

		m_pos.y += m_vec.y;

		// �c���瓖�����Ă��邩�ǂ������m�F����
		if (map.IsCol(GetRect(), chipRect, m_camera))
		{
			// �㉺�ǂ������瓖��������

			// �v���C���[���������Ɉړ����Ă���
			if (m_vec.y > 0.0f)
			{
				// ���ɓ������Ă���̂ŏ�ɉ����߂�
				m_pos.y = chipRect.top - 1;
				m_isJump = false;
			}
			// �v���C���[��������Ɉړ����Ă���
			else if (m_vec.y < 0.0f)
			{
				// �V��ɓ������Ă���̂ŉ��ɉ����߂�
				m_pos.y = chipRect.bottom + kVerticalHit;
				m_vec.y *= -1.0f;
			}
		}
		else
		{
			m_isJump = true;
		}
	}

	// �O�t���[���ŃW�����v���Ă��������m�F
	if (input.IsTrigger(PAD_INPUT_1))
	{
		m_isLastJumpButton = true;
	}
	else
	{
		m_isLastJumpButton = false;
	}

	// �U��
	if (input.IsTrigger(PAD_INPUT_2))
	{
		for (int i = 0; i < kShot; ++i)
		{
			if (!m_shot[i].m_isShotFlag)
			{
				// �A�j���[�V�����؂�ւ�
				m_isAtk = true;

				// �e�̈ʒu���v���C���[�̈ʒu�ɕ␳
				m_shot[i].m_pos = m_pos + m_camera.GetDrawOffset();

				// �e��\��
				m_shot[i].m_isShotFlag = true;

				// �e�̌������v���C���[�Ɠ��������ɕ␳
				m_shot[i].m_isDirLeft = m_isShotDirRight;

				// �e��1���o���ă��[�v���甲����
				break;
			}
		}
	}
	else
	{
		m_isAtk = false;
	}

	// �E�[�ɍs�����獶�[��
	if (m_pos.x <= kLeftEndWidth)
	{
		m_pos.x = kRightEndWidth;
	}
	// ���[�ɍs������E�[��
	else if (m_pos.x >= kRightEndWidth)
	{
		m_pos.x = kLeftEndWidth;
	}

	// HP���Ȃ��Ȃ����玀��
	if (m_hp <= 0)
	{
		DeleteGraph(m_handleIdle);
		DeleteGraph(m_handleRun);
		DeleteGraph(m_handleAtk);
		DeleteGraph(m_handleDeath);
	}

	// �e�𔭎�
	for (int i = 0; i < kShot; i++)
	{	
		m_shot[i].Update(boss, enemy1);
	}
}

void Player::Draw(Camera& camera)
{
	//Vec3 drawPos = m_camera.Capture(m_pos);

	// �v���C���[�̓����蔻��̕\��
	if (m_hp >= 0)
	{
		Vec3 camOffset = camera.GetDrawOffset();
		DrawBox(GetLeft() + camOffset.x, GetTop() + camOffset.y, GetRight() + camOffset.x, GetBottom() + camOffset.y, 0xff0000, true);
	}

	// �v���C���[�̃A�j���[�V�����؂�ւ�
	// ����
	if (m_isRun)
	{
		m_runAnim.Play(m_pos + camera.GetDrawOffset(), m_isDirLeft);
	}
	// �U��
	else if (m_isAtk)
	{
		m_atkAnim.Play(m_pos + camera.GetDrawOffset(), m_isDirLeft);
	}
	// �ҋ@
	else
	{
		m_idleAnim.Play(m_pos + camera.GetDrawOffset(), m_isDirLeft);
	}

	// �V���b�g
	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Draw();
	}

	//DrawFormatString(0, 0, 0xffffff, "PlayerPos.X=%f,Y=%f", m_pos.x, m_pos.y);
	//DrawFormatString(0, 15, 0xffffff, "DrawPos.X=%f,Y=%f", drawPos.x, m_pos.y);
}

float Player::GetLeft()
{
	return (m_pos.x - 40);
}

float Player::GetRight()
{
	return (m_pos.x + 40);
}

float Player::GetTop()
{
	return (m_pos.y - 10);
}

float Player::GetBottom()
{
	return (m_pos.y + kGraphHeight - 35);
}

Rect Player::GetRect()
{
	// �v���C���[�̋�`�����蔻����쐬
	Rect rect;
	rect.top = GetTop();
	rect.bottom = GetBottom();
	rect.right = GetRight();
	rect.left = GetLeft();
	return rect;
}