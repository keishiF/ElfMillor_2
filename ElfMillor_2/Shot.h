#pragma once
#include "Vec3.h"

class Input;
class Vec3;
class Boss;
class Enemy1;

class Shot
{
public:
	Shot();
	~Shot();

	void Init();
	void Update(Boss& boss, Enemy1 & enemy1);
	void Draw();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	// 弾が出ているかどうかのフラグ
	bool m_isShotFlag;

	// 弾の位置座標
	Vec3 m_pos;
	Vec3 m_velocity;

	// 左向きかどうか
	bool m_isDirLeft;

private:
	// 弾のグラフィックハンドル
	int m_handle;

	// 弾のグラフィックサイズ
	int m_graphWidth;
	int m_graphHeight;
};