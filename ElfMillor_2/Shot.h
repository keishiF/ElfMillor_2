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

	// �e���o�Ă��邩�ǂ����̃t���O
	bool m_isShotFlag;

	// �e�̈ʒu���W
	Vec3 m_pos;
	Vec3 m_velocity;

	// ���������ǂ���
	bool m_isDirLeft;

private:
	// �e�̃O���t�B�b�N�n���h��
	int m_handle;

	// �e�̃O���t�B�b�N�T�C�Y
	int m_graphWidth;
	int m_graphHeight;
};