#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"

#include <cassert>

namespace
{
	// ��ʒ���
	constexpr int kGameScreenHalfWidth = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;

	// ��ʒ[
	constexpr int kLeftEndWidth = 0;
	constexpr int kRightEndWidth = 1280;

	// �v���C���[�̏���HP
	constexpr int kMaxHp = 5;

	// �v���C���[�̈ړ����x
	constexpr int kSpeed = 5.0f;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimFrame = 11;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 8;

	// �L�����N�^�[�̃O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 160;
	constexpr int kGraphHeight = 128;

	// �ҋ@�A�j���[�V�����̃t���[����
	constexpr int kIdleAnimNum = 8;
	// ����A�j���[�V�����̃t���[����
	constexpr int kRunAnimNum = 8;
	// �U���A�j���[�V�����̃t���[����
	constexpr int kAtkAnimNum = 13;
	// ���S�A�j���[�V�����̃t���[����
	constexpr int kDeathAnimNum = 10;
}

Player::Player() :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleAtk(-1),
	m_handleDeath(-1),
	m_animFrame(0),
	m_isRun(false),
	m_isAtk(false),
	m_isDeath(false),
	m_pos(kGameScreenHalfWidth,kGameScreenHalfHeight),
	m_isDirLeft(false),
	m_blinkFrame(0),
	m_hp(kMaxHp),
	m_isLastJump(false)
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
}

void Player::End()
{	
}

void Player::Update(Input& input)
{
	// �A�j���[�V�����̍X�V
	m_animFrame++;
	int totalFrame = kIdleAnimNum * kSingleAnimFrame;
	if (m_isRun)
	{
		totalFrame = kRunAnimNum * kSingleAnimFrame;
	}
	if (m_isAtk)
	{
		totalFrame = kAtkAnimNum * kSingleAnimFrame;
	}
	if (m_isDeath)
	{
		totalFrame = kDeathAnimNum * kSingleAnimFrame;
	}

	if (m_animFrame >= totalFrame)
	{
		m_animFrame = 0;
	}

	// ����
	m_isRun = false;
	// ������
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_pos.x -= kSpeed;
	}
	// �E����
	else if (input.IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_pos.x += kSpeed;
	}
	// �����ĂȂ�
	else
	{
		m_isRun = false;
	}
	// �U��
	if (input.IsTrigger(PAD_INPUT_2))
	{
		m_isAtk = true;
	}
	// �U�����Ă��Ȃ�
	else
	{
		m_isAtk = false;
	}
	// ���S
	if (input.IsPress(PAD_INPUT_4))
	{
		m_isDeath = true;
	}
	// �����Ă���
	else
	{
		m_isDeath = false;
	}

	if (m_pos.x <= kLeftEndWidth)
	{
		m_pos.x = kRightEndWidth;
	}
	else if (m_pos.x >= kRightEndWidth)
	{
		m_pos.x = kLeftEndWidth;
	}
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;

	// �\������O���t�B�b�N���v���C���[�̏�Ԃɉ����ĕύX
	// �������Ă��Ȃ��Ƃ��ɑҋ@�A�j���[�V����
	int useHandle = m_handleIdle;

	// ����
	if (m_isRun)
	{
		useHandle = m_handleRun;
	}
	// �U��
	if (m_isAtk)
	{
		useHandle = m_handleAtk;
	}
	// ���S
	if (m_isDeath)
	{
		useHandle = m_handleDeath;
	}

	// �`��
	DrawRectRotaGraph(m_pos.x, m_pos.y,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0f, 0.0f,
		useHandle, true, m_isDirLeft);
}