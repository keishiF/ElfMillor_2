#pragma once
#include "Vec3.h"
#include "Animation.h"

class EnemyBase
{
public:
	// �\���ʒu
	Vec3 m_pos;
	Vec3 m_vec;

protected:
	// �O���t�B�b�N
	int m_handleIdle;

	// �S�̂̃t���[���𐔂���
	int m_animAllFrame;

	// HP
	int m_hp;

	// �A�j���[�V����
	Animation m_idleAnim;

	// ���������Ă邩�ǂ����̃t���O
	bool m_isDirLeft;

public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};