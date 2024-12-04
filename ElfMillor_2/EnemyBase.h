#pragma once
#include "Vec3.h"
#include "Animation.h"

class EnemyBase
{
public:
	// 表示位置
	Vec3 m_pos;
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
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};