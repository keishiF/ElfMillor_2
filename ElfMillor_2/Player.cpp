#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"
#include "Boss.h"

#include <cassert>

namespace
{
	// �����ʒu
	constexpr int kPlayerPosX = 270;
	constexpr int kPlayerPosY = 460;

	// ��ʒ[
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	constexpr int kFieldHeight = 352;

	// �v���C���[�̏���HP
	constexpr int kMaxHp = 5;

	// �v���C���[�̈ړ����x
	constexpr float kSpeed = 5.0f;

	// �d��
	constexpr float kGravity = 0.4f;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 8;

	// �L�����N�^�[�̃O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 160;
	constexpr int kGraphHeight = 128;

	// �ҋ@�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimNum = 8;
	// ����A�j���[�V�����̃R�}��
	constexpr int kRunAnimNum = 8;
	// �U���A�j���[�V�����̃R�}��
	constexpr int kAtkAnimNum = 13;
	// ���S�A�j���[�V�����̃R�}��
	constexpr int kDeathAnimNum = 10;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExpRate = 2.0f;
}

Player::Player() :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleAtk(-1),
	m_handleDeath(-1),
	m_isRun(false),
	m_isJump(false),
	m_isAtk(false),
	m_isDeath(false),
	m_jumpSpeed(0.0f),
	m_pos(kPlayerPosX, kPlayerPosY),
	m_vec(),
	m_isDirLeft(false),
	m_isShotDirRight(true),
	m_blinkFrame(0),
	m_hp(kMaxHp),
	m_isLastJump(false),
	m_isLastJumpButton(false),
	m_shot(),
	m_idleAnim(),
	m_runAnim(),
	m_atkAnim()
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

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kIdleAnimNum);
	m_runAnim.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kRunAnimNum);
	m_atkAnim.Init(m_handleAtk, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kAtkAnimNum);
}

void Player::End()
{	
}

void Player::Update(Input& input, Boss& boss)
{
	m_idleAnim.Update();
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
	// �����ĂȂ�
	else
	{
		m_isRun = false;
		m_vec.x = 0;
	}

	// �W�����v
	if (input.IsTrigger(PAD_INPUT_1))
	{
		if (!m_isJump)
		{
			m_isJump = true;
			m_jumpSpeed = -10.0f;
		}
	}
	if (m_isJump)
	{
		m_pos.y += m_jumpSpeed;
		m_jumpSpeed += kGravity;

		if (m_jumpSpeed > 0.0f)
		{
			if (m_pos.y >= kPlayerPosY)
			{
				m_isJump = false;
				m_jumpSpeed = 0.0f;
			}
		}
	}
	if (input.IsTrigger(PAD_INPUT_1))
	{
		m_isLastJumpButton = true;
	}
	else
	{
		m_isLastJumpButton = false;
	}

	// �ړ�����
	m_vec = m_vec.GetNormalize() * kSpeed;
	m_pos += m_vec;

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
				m_shot[i].m_pos = m_pos;

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

	if (m_pos.x <= kLeftEndWidth)
	{
		m_pos.x = kRightEndWidth;
	}
	else if (m_pos.x >= kRightEndWidth)
	{
		m_pos.x = kLeftEndWidth;
	}

	for (int i = 0; i < kShot; i++)
	{	
		m_shot[i].Update(boss);
	}
}

void Player::Draw()
{
	if (m_isRun)
	{
		m_runAnim.Play(m_pos, m_isDirLeft);
	}
	else if (m_isAtk)
	{
		m_atkAnim.Play(m_pos, m_isDirLeft);
	}
	else
	{
		m_idleAnim.Play(m_pos, m_isDirLeft);
	}
	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Draw();
	}
}