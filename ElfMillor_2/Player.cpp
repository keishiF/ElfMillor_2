#include "Player.h"
#include "DxLib.h"
#include <cassert>

namespace
{
	// �v���C���[�̏���HP
	constexpr int kMaxHp = 5;

	// �v���C���[�̈ړ����x
	constexpr int kSpeed = 0.2f;

	// �e�A�j���[�V�����̃R�}��
	constexpr int kIdleAnimFrame = 11;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kSingleAnimFrame = 5;

	// �L�����N�^�[�̃O���t�B�b�N�̃T�C�Y
	constexpr int kGraphWidth = 80;
	constexpr int kGraphHeight = 80;

	// �e�A�j���[�V�����̃t���[����
	constexpr int kIdleAnimNum = 9;
}

Player::Player() :
	m_handleIdle(-1),
	m_animFrame(0),
	m_isJump(false),
	m_blinkFrame(0),
	m_hp(kMaxHp),
	m_deadFrame(0),
	m_isLastJump(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
	// �O���t�B�b�N�̓ǂݍ���
	m_handleIdle = LoadGraph("img/Player/Idle.png");
	assert(m_handleIdle != -1);
}

void Player::End()
{
	// �O���t�B�b�N�̉��
	DeleteGraph(m_handleIdle);
}

void Player::Update()
{
	// �A�j���[�V�����̍X�V
	m_animFrame++;

	int totalFrame = kIdleAnimNum * kSingleAnimFrame;

	if (m_animFrame >= totalFrame)
	{
		m_animFrame = 0;
	}
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;

	// ���\���ʒu
	int playerPosX = 460;
	int playerPosY = 345;

	int useHandle = m_handleIdle;

	DrawRectRotaGraph(playerPosX, playerPosY,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0f, 0.0f,
		useHandle, true, false);
}