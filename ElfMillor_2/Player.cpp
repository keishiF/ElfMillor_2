#include "Player.h"
#include "Camera.h"
#include "Map.h"

#include "game.h"
#include "Input.h"

#include "DxLib.h"
#include <cassert>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

namespace
{
	// �����ʒu
	constexpr int kPlayerInitPosX = 360;
	constexpr int kPlayerInitPosY = 7600;

	// ��ʒ[
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	// �v���C���[�̏���HP
	constexpr int kDefaultHp = 10;

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
	constexpr int kDeadAnimNum = 10;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExtRate = 1.75f;

	// �O���t�B�b�N�̉�]��
	constexpr float kRotaRate = 0.0f;

	// �m�b�N�o�b�N����
	constexpr int kBesideHit = 35;
	constexpr int kVerticalHit = 1;

	// �_���[�W�H�������̖��G����
	constexpr int kDamageBlinkFrame = 30;

	// ���S���̉��o�Đ�����
	constexpr int kDeadFrame = 80;
}

Player::Player(std::weak_ptr<Camera> camera) :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleDead(-1),
	m_seHandle(-1),
	m_isRun(false),
	m_isJump(false),
	m_isDead(false),
	m_jumpSpeed(-10.5f),
	m_jumpCount(0),
	m_isDirLeft(false),
	m_isUp(false),
	m_blinkFrameCount(0),
	m_deadFrameCount(0),
	m_hp(kDefaultHp),
	m_isLastJumpButton(false),
	m_isClearFlag(false),
	m_shot(),
	m_idleAnim(),
	m_runAnim(),
	m_deadAnim(),
	// ���N���X�̏�����
	GameObject(Vec3(kPlayerInitPosX, kPlayerInitPosY), camera)
{
}

Player::~Player()
{
	// �O���t�B�b�N�̉��
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleDead);

	// SE�̉��
	DeleteSoundMem(m_seHandle);
}

void Player::Init()
{
	// �O���t�B�b�N�̓ǂݍ���
	m_handleIdle = LoadGraph("data/image/Player/Idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("data/image/Player/Run.png");

	m_handleDead = LoadGraph("data/image/Player/Dead.png");
	assert(m_handleDead != -1);

	// SE�̓ǂݍ���
	m_seHandle = LoadSoundMem("data/sound/shotSE3.mp3");
	assert(m_seHandle != -1);

	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Init();
	}

	m_hp = kDefaultHp;

	m_isClearFlag = false;

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kIdleAnimNum);
	m_runAnim.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kRunAnimNum);
	m_deadAnim.Init(m_handleDead, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kDeadAnimNum);
}

void Player::Update(Input& input, std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
	std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, Map& map)
{
	// ���G���Ԃ̍X�V
	UpdateBlinkFrame();

	m_idleAnim.Update();

	m_isRun = false;
	m_vec.x = 0.0f;

	if (m_isRun)
	{
		m_runAnim.Update();
	}

	HandleMovement(input, map);

	// �O�t���[���ŃW�����v���Ă��������m�F
	m_isLastJumpButton = input.IsTrigger(PAD_INPUT_1);

	// �����
	if (input.IsPress(PAD_INPUT_UP))
	{
		m_isUp = true;
	}
	else
	{
		m_isUp = false;
	}

	// �U��
	HandleAttack(input);

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

	// �e�𔭎�
	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Update(groundEnemy, flyingEnemy, m_camera, map);
	}
}

void Player::Draw()
{
	// �_�ŏ���
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	Vec3 camOffset = m_camera.lock()->GetDrawOffset();
	camOffset.x = 0;

	// �v���C���[�̃A�j���[�V�����؂�ւ�
	// ����
	if (m_isRun)
	{
		m_runAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
	// ����
	else if (m_isDead)
	{
		m_deadAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
	// �ҋ@
	else
	{
		m_idleAnim.Play(m_pos + camOffset, m_isDirLeft);
	}

#ifdef DISP_COLLISION
	// �v���C���[�̓����蔻��̕\��
	if (m_hp >= 0)
	{
		DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop() + camOffset.y),
			static_cast<int>(GetRight()), static_cast<int>(GetBottom() + camOffset.y), 0xff0000, false);
	//	DrawCircle(static_cast<int>(m_pos.x) + camOffset.x,
	//		static_cast<int>(m_pos.y) + camOffset.y, 10, 0xff00ff, false);
		DrawFormatString(5, 30, 0xffffff, "PlayerPos.X=%f,Y=%f", m_pos.x, m_pos.y);
	}
#endif

	// �V���b�g
	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Draw(m_camera);
	}

	DrawFormatString(5, 0, 0xffffff, "Hp = %d", m_hp);
}

void Player::DeadUpdate()
{
	m_deadAnim.Update();
	if (m_deadAnim.IsEnd())
	{
		return;
	}
}

void Player::OnDamage()
{
#ifdef DISP_COLLISION
	printfDx("�_���[�W�I\n");
#endif

	// ���Ƀ_���[�W���󂯂Ă���(���G���Ԃ�)
	// �ēx�_���[�W���󂯂邱�Ƃ͖���
	if (m_blinkFrameCount > 0) return;

	// ���G����(�_�ł��鎞��)��ݒ肷��
	m_blinkFrameCount = kDamageBlinkFrame;

	// �_���[�W���󂯂�
	m_hp--;
}

float Player::GetLeft()
{
	return (m_pos.x - 30);
}

float Player::GetRight()
{
	return (m_pos.x + 30);
}

float Player::GetTop()
{
	return (m_pos.y +5);
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

void Player::UpdateBlinkFrame()
{
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}
}

void Player::HandleMovement(Input& input, Map& map)
{
	// ������
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_vec.x -= kSpeed;
	}
	// �E����
	else if (input.IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_vec.x += kSpeed;
	}

	// �W�����v
	if (m_isJump)
	{
		HandleJump(map);
	}
	else
	{
		HandleGroundMovement(input, map);
	}
}

void Player::HandleJump(Map& map)
{
	// �󒆂ɂ���Ƃ��̏���

	// ���t���[���d�͂ɂ���ĉ������ɉ�������
	m_vec.y += kGravity;

	// �ړ�����
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
			m_isJump = false;
			// ���ɓ������Ă���̂ŏ�ɉ����߂�
			m_pos.y -= m_vec.y;
			m_vec.y = 0.0f;
		}
		// �v���C���[��������Ɉړ����Ă���
		else if (m_vec.y < 0.0f)
		{
			// �V��ɓ������Ă���̂ŉ��ɉ����߂�
			m_pos.y = chipRect.bottom + kVerticalHit;
			m_vec.y = 0.0f;
		}
	}

	// ������͂��蔲���A�ォ��͏��鏰�̔���
	if (map.IsCol2(GetRect(), chipRect, m_camera))
	{
		// �㉺�ǂ������瓖��������

		// �v���C���[���������Ɉړ����Ă���
		if (m_vec.y > 0.0f)
		{
			// �O�t���[���̃v���C���[�̈ʒu
			Vec3 posTemp = m_pos - m_vec;
			// �v���C���[�̒��_���珰�̒��S�܂ł̋���
			float playerMapDistance = chipRect.GetCenter().y - posTemp.y;
			// ���������Ăق�������
			float distance = MapConsts::kMapChipSize * 0.5f + GetBottom() - GetTop() - 5;
			// �v���C���[�̒��_���珰�̒��S�܂ł̋��������������Ăق����������傫��
			if (playerMapDistance > distance)
			{
				m_isJump = false;
				// ���ɓ������Ă���̂ŏ�ɉ����߂�
				m_pos.y -= m_vec.y;
				m_vec.y = 0.0f;
			}
		}
	}

	// �_���[�W���Ƃ̓����蔻��
	if (map.IsDamageCol(GetRect(), chipRect, m_camera))
	{
		OnDamage();
	}

	// �N���A�̃t���O�ƂȂ��Ă��錮�Ƃ̓����蔻��
	if (map.IsClearCol(GetRect(), chipRect, m_camera))
	{
		m_isClearFlag = true;
	}
}

void Player::HandleGroundMovement(Input& input, Map& map)
{
	// �W�����v
	if (input.IsPress(PAD_INPUT_1) && !m_isJump)
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
			m_isJump = false;
			// ���ɓ������Ă���̂ł��̍����ɍ��킹��
			m_pos.y = chipRect.top;
		}
		// �v���C���[��������Ɉړ����Ă���
		else if (m_vec.y < 0.0f)
		{
			// �V��ɓ������Ă���̂ŉ��ɉ����߂�
			m_pos.y = chipRect.bottom + kVerticalHit;
			m_vec.y = 0.0f;
		}
	}
	else
	{
		m_isJump = true;
	}

	// ������͂��蔲���A�ォ��͏��鏰�̔���
	if (map.IsCol2(GetRect(), chipRect, m_camera))
	{
		// �㉺�ǂ������瓖��������

		// �v���C���[���������Ɉړ����Ă���
		if (m_vec.y > 0.0f)
		{
			// �O�t���[���̃v���C���[�̈ʒu
			Vec3 posTemp = m_pos - m_vec;
			// �v���C���[�̒��_���珰�̒��S�܂ł̋���
			float playerMapDistance = chipRect.GetCenter().y - posTemp.y;
			// ���������Ăق�������
			float distance = MapConsts::kMapChipSize * 0.5f + GetBottom() - GetTop() - 5;
			// �v���C���[�̒��_���珰�̒��S�܂ł̋��������������Ăق����������傫��
			if (playerMapDistance > distance)
			{
				m_isJump = false;
				// ���ɓ������Ă���̂ł��̍����ɍ��킹��
				m_pos.y = chipRect.top;
			}
		}
	}
	else
	{
		m_isJump = true;
	}

	// �_���[�W���Ƃ̓����蔻��
	if (map.IsDamageCol(GetRect(), chipRect, m_camera))
	{
		OnDamage();
	}

	// �N���A�̃t���O�ƂȂ��Ă��錮�Ƃ̓����蔻��
	if (map.IsClearCol(GetRect(), chipRect, m_camera))
	{
		m_isClearFlag = true;
	}
}

void Player::HandleAttack(Input& input)
{
	if (input.IsTrigger(PAD_INPUT_2))
	{
		for (int i = 0; i < kShot; ++i)
		{
			if (!m_shot[i].m_isShot)
			{
				// �V���b�g���̉��̍Đ�
				PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK, true);

				// �e�̈ʒu���v���C���[�̈ʒu�ɕ␳
				m_shot[i].m_pos.x = m_pos.x;
				m_shot[i].m_pos.y = m_pos.y + 35;

				// �e��\��
				m_shot[i].m_isShot = true;

				// �e�̌������v���C���[�Ɠ��������ɕ␳
				m_shot[i].m_isDirLeft = m_isDirLeft;

				// ����͂���Ă��邩�ǂ������v���C���[�Ɠ�����Ԃɂ���
				m_shot[i].m_isUp = m_isUp;

				// �e��1���o���ă��[�v���甲����
				break;
			}
		}
	}
}