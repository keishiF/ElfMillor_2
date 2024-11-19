#pragma once
#include "Vec3.h"
#include "Bullet.h"
#include "Animation.h"

#include <DxLib.h>

class Input;
class Vec3;

class Player
{
public:
	// �v���C���[�̃R���X�g���N�^�ƃf�X�g���N�^
	Player();
	~Player();

	// �v���C���[�̏������A�I�������A�X�V�����A�`�揈��
	void Init();
	void End();
	void Update(Input& input);
	void Draw();

private:
	// �v���C���[�̃O���t�B�b�N�n���h��
	int m_handleIdle;
	int m_handleRun;
	int m_handleAtk;
	int m_handleDeath;

	// �A�j���[�V�����̃t���[�����𐔂���
	int m_animAllFrame;

	// �e�A�j���[�V�����Ɋւ���t���O
	bool m_isRun;
	bool m_isJump;
	bool m_isAtk;
	bool m_isDeath;

	// �W�����v�X�s�[�h
	float m_jumpSpeed;

	// �v���C���[�̕\���ʒu
	Vec3 m_pos;
	Vec3 m_vec;

	// �v���C���[�����������ǂ���
	bool m_isDirLeft;
	// �e���E�����ɂ��邩�ǂ���
	bool m_isBulletDirRight;

	// ��e���̓_��
	int m_blinkFrame;

	// �v���C���[��HP
	int m_hp;

	// �����W�����v���Ȃ��悤�ɊǗ�����
	bool m_isLastJump;
	bool m_isLastJumpButton;

	// �e�̏��
	Bullet* m_pBullet;

	Animation m_idleAnim;
	Animation m_runAnim;
	Animation m_atkAnim;

};