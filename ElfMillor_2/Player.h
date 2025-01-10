#pragma once
#include "Vec3.h"
#include "Animation.h"
#include "Shot.h"
#include "GameObject.h"
#include "Rect.h"

class Vec3;
class Boss;
class Enemy1;
class Map;
class Camera;

namespace
{
	constexpr int kShot = 15;
}

class Player : public GameObject
{
public:
	// �v���C���[�̃R���X�g���N�^�ƃf�X�g���N�^
	Player(Camera& camera);
	~Player();

	// �v���C���[�̏������A�I�������A�X�V�����A�`�揈��
	void Init();
	void End();
	void Update(Input& input, Boss& boss, Enemy1& enemy1, Map& map);
	void Draw(Camera& camera);

	// �����蔻��̂��߂̍��W�擾
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	// �v���C���[�̋�`���擾
	Rect GetRect();

	// �v���C���[�̕\���ʒu
	Vec3 m_vec;

private:
	// �v���C���[�̃O���t�B�b�N�n���h��
	int m_handleIdle;
	int m_handleRun;
	int m_handleAtk;
	int m_handleDeath;

	// �e�A�j���[�V������؂�ւ���t���O
	bool m_isRun;
	bool m_isJump;
	bool m_isAtk;
	bool m_isDeath;

	// �W�����v�X�s�[�h
	float m_jumpSpeed;
	int m_jumpCount;

	// �v���C���[�����������ǂ���
	bool m_isDirLeft;
	// �e���E�����ɂ��邩�ǂ���
	bool m_isShotDirRight;

	// �n�ʂɓ������Ă��邩�ǂ����̃t���O
	bool m_isGroundHit;

	// �V��Ƃ̓����蔻��
	bool m_isCeilingHit;

	// ��e���̓_��
	int m_blinkFrame;

	// �v���C���[��HP
	int m_hp;

	// �����W�����v���Ȃ��悤�ɊǗ�����
	bool m_isLastJump;
	bool m_isLastJumpButton;

	// �e�̏��
	Shot m_shot[kShot];

	// �e�A�j���[�V����
	Animation m_idleAnim;
	Animation m_runAnim;
	Animation m_atkAnim;
};