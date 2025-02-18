#pragma once

#include "Vec3.h"

// 矩形のデータ管理
class Rect
{
public:
	// 矩形情報
	float top;
	float bottom;
	float left;
	float right;

	Rect() : //コンストラクタで初期化
		top(0),
		bottom(0),
		left(0),
		right(0)
	{
	}
	// 横幅情報
	float GetWidth() const { return right - left; }
	// 縦幅情報
	float GetHeight() const { return bottom - top; }

	// 各座標取得
	Vec3 GetCenter();

	// 当たり(重なり)判定
	bool IsCol(const Rect& rect);
	// 座標が矩形内に含まれるかどうか判定
};