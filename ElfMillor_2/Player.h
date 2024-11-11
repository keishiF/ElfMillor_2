#pragma once
#include "Vec3.h"

class Input;

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
	/*int m_handleJump;
	int m_handleAtk;
	int m_handleUpAtk;*/

	// �A�j���[�V�����̃t���[�����𐔂���
	int m_animFrame;

	bool m_isRun;

	// �v���C���[�̕\���ʒu
	//Vec3 m_pos;

	// �W�����v
	bool m_isJump;

	// ��e���̓_��
	int m_blinkFrame;

	// �v���C���[��HP
	int m_hp;

	// ���S���o�p�̃t���[��
	int m_deadFrame;

	// �����W�����v���Ȃ��悤�ɊǗ�����
	bool m_isLastJump;
};