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
	// �S�̂̃t���[���𐔂���
	int m_animAllFrame = 0;

	// �A�j���[�V����
	Animation m_idleAnim;
	Animation m_runAnim;

	// ���������Ă邩�ǂ����̃t���O
	bool m_isDirLeft;

public:
	EnemyBase(Vec3 initPos, Camera& camera) :
		GameObject(initPos, camera)
	{
	}
	virtual ~EnemyBase() {}

	virtual void Init(float posX, float posY) { m_pos.x = posX, m_pos.y = posY; };
	virtual void Update() = 0;
	virtual void Draw() = 0;
};