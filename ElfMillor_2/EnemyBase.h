#pragma once
#include "Vec3.h"
#include "Animation.h"
#include "GameObject.h"

class Camera;

class EnemyBase : public GameObject
{
public:
	EnemyBase(Vec3 initPos, Camera& camera) :
		m_hp(),
		m_handle(),
		m_idleAnim(),
		m_runAnim(),
		GameObject(initPos, camera)
	{
	}
	virtual ~EnemyBase() {}

	void SetHandle(int handle) { m_handle = handle; }

	virtual void Init(float posX, float posY) { m_pos.x = posX, m_pos.y = posY; };
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// �\���ʒu
	Vec3 m_vec;

	// HP
	int m_hp;

protected:
	// �O���t�B�b�N
	int m_handle;

	// �A�j���[�V����
	Animation m_idleAnim;
	Animation m_runAnim;
};