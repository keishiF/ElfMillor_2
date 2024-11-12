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
	constexpr int kSingleAnimFrame = 5;

	// �L�����N�^�[�̃O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 80;
	constexpr int kGraphHeight = 80;

	// �ҋ@�A�j���[�V�����̃t���[����
	constexpr int kIdleAnimNum = 9;
	// ����A�j���[�V�����̃t���[����
	constexpr int kRunAnimNum = 5;
	// �U���A�j���[�V�����̃t���[����
	constexpr int kAtkAnimNum = 12;
}

Player::Player() :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleAtk(-1),
	m_animFrame(0),
	m_isRun(false),
	m_isAtk(false),
	m_pos(kGameScreenHalfWidth,kGameScreenHalfHeight),
	m_isDirLeft(false),
	m_isJump(false),
	m_blinkFrame(0),
	m_hp(kMaxHp),
	m_deadFrame(0),
	m_isLastJump(false)
{
}

Player::~Player()
{
	// �O���t�B�b�N�̉��
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleAtk);
}

void Player::Init()
{
	// �O���t�B�b�N�̓ǂݍ���
	m_handleIdle = LoadGraph("img/Player/Idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("img/Player/Run.png");
	assert(m_handleRun != -1);

	m_handleAtk = LoadGraph("img/Player/Atk.png");
	assert(m_handleAtk != -1);
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

	if (m_animFrame >= totalFrame)
	{
		m_animFrame = 0;
	}

	m_isRun = false;
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_pos.x -= kSpeed;
	}
	else if (input.IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_pos.x += kSpeed;
	}
	else
	{
		m_isRun = false;
	}

	if (input.IsPress(PAD_INPUT_1))
	{
		m_isAtk = true;
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
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;

	int useHandle = m_handleIdle;
	if (m_isRun)
	{
		useHandle = m_handleRun;
	}
	if (m_isAtk)
	{
		useHandle = m_handleAtk;
	}

	DrawRectRotaGraph(m_pos.x, m_pos.y,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0f, 0.0f,
		useHandle, true, m_isDirLeft);
}