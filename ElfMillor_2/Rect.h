#pragma once

#include "Vec3.h"

// 矩形のデータ管理
class Rect
{
public:
	float top;
	float bottom;
	float left;
	float right;
public:
	Rect() : //コンストラクタで初期化
		top(0),
		bottom(0),
		left(0),
		right(0)
	{
	}
	float getWidth() const { return right - left; }
	float getHeight() const { return bottom - top; }

	// 各座標取得
	Vec3 getCenter();

	// 当たり(重なり)判定
	bool isCol(const Rect& rect);
	// 座標が矩形内に含まれるかどうか判定
};