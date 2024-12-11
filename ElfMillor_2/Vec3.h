#pragma once
#include <cmath>

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

	Vec3 operator+=(const Vec3& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vec3 operator+(const Vec3& vec) const
	{
		Vec3 temp{ x + vec.x, y + vec.y };
		return temp;
	}

	Vec3 operator-=(const Vec3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vec3 operator-(const Vec3& vec) const
	{
		Vec3 temp{ x - vec.x, y - vec.y };
		return temp;
	}

	Vec3 operator-() const
	{
		return Vec3(-x, -y);
	}

	Vec3 operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Vec3 operator*(float scale) const
	{
		Vec3 temp{ x * scale,y * scale };
		return temp;
	}

	Vec3 operator/=(float scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}

	Vec3 operator/(float scale) const
	{
		Vec3 temp{ x / scale,y / scale };
		return temp;
	}

	float Length() const
	{
		return sqrtf(x * x + y * y);
	}

	// 自身の長さを1にする(正規化)
	void Normalize()
	{
		float len = Length();
		if (len <= 0)
		{
			// ゼロベクトルはそのまま
			return;
		}
		x /= len;
		y /= len;
	}

	// 自身の長さを1にしたベクトルの取得
	// (この関数を読んでも自身の長さは変化しない)
	Vec3 GetNormalize() const
	{
		float len = Length();
		if (len <= 0.0f)
		{
			return *this;
		}
		return (*this) / len;
	}
};