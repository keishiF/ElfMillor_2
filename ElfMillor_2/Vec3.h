#pragma once

// 3次元ベクトルクラス
class Vec3
{
public:
	float x;
	float y;
//	float z;

public:
	Vec3()
	{
		x = 0.0f;
		y = 0.0f;
//		z = 0.0f;
	}
	Vec3(float _x, float _y):
		x(_x),
		y(_y)
	{

	}
};