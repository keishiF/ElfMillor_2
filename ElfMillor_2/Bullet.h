#pragma once
#include "Vec3.h"

class Input;
class Vec3;

class Bullet
{
public:
	Bullet();
	~Bullet();

	void Init();
	void Draw(Bullet& bullet);

	// 弾が出ているかどうかのフラグ
	bool m_isShotFlag;

	// 弾の位置座標
	Vec3 m_pos;

private:
	// 弾のグラフィックハンドル
	int m_handle;

	// 弾のグラフィックサイズ
	int m_graphWidth;
	int m_graphHeight;
};