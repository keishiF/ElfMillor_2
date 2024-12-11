#pragma once
#include "Vec3.h"
#include "Animation.h"
#include "GameObject.h"

class Camera;

class EnemyBase : public GameObject
{
public:
	// 表示位置
	Vec3 m_vec;

	// HP
	int m_hp;

protected:
	// グラフィック
	int m_handleIdle;

	// 全体のフレームを数える
	int m_animAllFrame;

	// アニメーション
	Animation m_idleRun;

	// 左を向いてるかどうかのフラグ
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