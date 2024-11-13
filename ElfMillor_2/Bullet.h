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

private:
	// �e�̃O���t�B�b�N�n���h��
	int m_handle;

	// �e�̈ʒu���W
	Vec3 m_pos;

	// �e�̃O���t�B�b�N�T�C�Y
	int m_graphWidth;
	int m_graphHeight;
};