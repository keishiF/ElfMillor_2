#pragma once
#include <cmath>

// 3�����x�N�g���N���X
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

	// ���g�̒�����1�ɂ���(���K��)
	void Normalize()
	{
		float len = Length();
		if (len <= 0)
		{
			// �[���x�N�g���͂��̂܂�
			return;
		}
		x /= len;
		y /= len;
	}

	// ���g�̒�����1�ɂ����x�N�g���̎擾
	// (���̊֐���ǂ�ł����g�̒����͕ω����Ȃ�)
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