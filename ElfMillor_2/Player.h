#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Animation.h"
#include "GameObject.h"
#include "Shot.h"

class Boss;
class GroundEnemy;
class Map;
class Camera;

namespace
{
	constexpr int kShot = 5;
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
	void Draw(Camera& camera);
	void Update(Input& input, Boss& boss, GroundEnemy& groundEnemy, Map& map, Camera& camera);

	// ��Ԃ��Ƃ̃A�b�v�f�[�g
	void NormalUpdate(Input& input, Boss& boss, GroundEnemy& groundEnemy, Map& map, Camera& camera);
	void DeadUpdate(Camera& camera);
	
	// �_���[�W����
	void OnDamage();

	// �����蔻��̂��߂̍��W�擾
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	// �v���C���[�̋�`���擾
	Rect GetRect();

	// �v���C���[�̕\���ʒu
	Vec3 m_vec;

	// �v���C���[��HP���擾
	int GetHp() { return m_hp; };

private:
	// �v���C���[�̃O���t�B�b�N�n���h��
	int m_handleIdle;
	int m_handleRun;
	int m_handleAtk;
	int m_handleDead;

	// �e�A�j���[�V������؂�ւ���t���O
	bool m_isRun;
	bool m_isJump;
	bool m_isAtk;
	bool m_isDead;

	// �W�����v�X�s�[�h
	float m_jumpSpeed;
	int   m_jumpCount;

	// �v���C���[�����������ǂ���
	bool m_isDirLeft;

	// �n�ʂɓ������Ă��邩�ǂ����̃t���O
	bool m_isGroundHit;

	// �V��Ƃ̓����蔻��
	bool m_isCeilingHit;

	// ��e���̓_��
	int m_blinkFrameCount;

	// ���S���̃t���[��
	int m_deadFrameCount;

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
	Animation m_deadAnim;
};