#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"
#include "Boss.h"
#include "Enemy1.h"
#include "Map.h"

#include <cassert>

namespace
{
	// �����ʒu
	constexpr int kPlayerPosX = 0; // 270
	constexpr int kPlayerPosY = 0; // 460

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

	// �m�b�N�o�b�N����
	constexpr int kKnockBack = 30;
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
	m_isUp(false),
	m_blinkFrame(0),
	m_hp(kDefaultHp),
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

	// ����͂��Ă��邩�ǂ���
	if (input.IsTrigger(PAD_INPUT_UP))
	{
		m_isUp = true;
	}
	else
	{
		m_isUp = false;
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

				// �e�ɏ���͂��Ă��邩�ǂ�����������
				m_shot[i].m_isUp = m_isUp;

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

	//// �E�[�ɍs�����獶�[��
	//if (m_pos.x <= kLeftEndWidth)
	//{
	//	m_pos.x = kRightEndWidth;
	//}
	//// ���[�ɍs������E�[��
	//else if (m_pos.x >= kRightEndWidth)
	//{
	//	m_pos.x = kLeftEndWidth;
	//}

	// ��e
	if (enemy1.m_hp > 0)
	{
		if (GetRight() > enemy1.GetLeft() &&
			GetLeft() < enemy1.GetRight() &&
			GetTop() < enemy1.GetBottom() &&
			GetBottom() > enemy1.GetTop())
		{
			// HP�����炷
			m_hp--;

			// �����������肪���Ȃ������Ƀm�b�N�o�b�N
			if (m_pos.x < enemy1.m_pos.x)
			{
				m_pos.x -= kKnockBack;
				enemy1.m_pos.x += kKnockBack;
			}
			else if (m_pos.x > enemy1.m_pos.x)
			{
				m_pos.x += kKnockBack;
				enemy1.m_pos.x -= kKnockBack;
			}
		}
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

	// �}�b�v�Ƃ̓����蔻��
	for (int y = 0; y < kMapHeight; y++)
	{
		for (int x = 0; x < kMapWidth; x++)
		{
			for (int i = 0; i < _countof(kWhiteList); i++)
			{
				if (map.mapChips[y][x].chipNo == kWhiteList[i])
				{
					MapChip chip = map.mapChips[y][x];
					float chipBottom = chip.m_pos.y + kMapChipSize;
					float chipTop = chip.m_pos.y;
					float chipRight = chip.m_pos.x + kMapChipSize;
					float chipLeft = chip.m_pos.x;
					if (GetTop() < chipBottom ||
						GetBottom() > chipTop ||
						GetRight() > chipLeft ||
						GetLeft() < chipRight)
					{
						//printfDx("�������Ă�\n");
					}
					else
					{
						//printfDx("�������ĂȂ�\n");
					}
				}
			}
		}
	}
}

void Player::Draw()
{
	DrawBox(static_cast<int>(m_pos.x - kGraphWidth * 0.5f), static_cast<int>(m_pos.y), static_cast<int>(m_pos.x + kGraphWidth * 0.5f), static_cast<int>(m_pos.y + kGraphHeight), 0x0000ff, false);
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

float Player::GetLeft()
{
	return (m_pos.x - kGraphWidth * 0.5f);;
}

float Player::GetRight()
{
	return (m_pos.x + kGraphWidth * 0.5f);
}

float Player::GetTop()
{
	return (m_pos.y - kGraphHeight);
}

float Player::GetBottom()
{
	return (m_pos.y + kGraphHeight * 0.5f);
}