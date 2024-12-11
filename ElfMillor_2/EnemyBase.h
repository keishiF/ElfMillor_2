#pragma once
#include "Vec3.h"
#include "Animation.h"
#include "GameObject.h"

class Camera;

class EnemyBase : public GameObject
{
public:
	// �\���ʒu
	Vec3 m_vec;

	// HP
	int m_hp;

protected:
	// �O���t�B�b�N
	int m_handleIdle;

	// �S�̂̃t���[���𐔂���
	int m_animAllFrame;

	// �A�j���[�V����
	Animation m_idleRun;

	// ���������Ă邩�ǂ����̃t���O
	bool m_isDirLeft;

public:
	EnemyBase(Vec3 initPos, Camera& camera) :
		GameObject(initPos, camera)
	{
	}
	virtual ~EnemyBase() {}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};