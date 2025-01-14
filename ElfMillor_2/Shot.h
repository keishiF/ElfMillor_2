#pragma once
#include "Vec3.h"

class Input;
class Boss;
class Enemy1;
class Camera;

class Shot
{
public:
	Shot();
	~Shot();

	void Init();
	void Update(Boss& boss, Enemy1& enemy1, Camera& camera);
	void Draw(Camera& camera);

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

	// ����͂���Ă��邩�ǂ���
	bool m_isUp;

private:
	// �e�̃O���t�B�b�N�n���h��
	int m_handle;

	// �e�̃O���t�B�b�N�T�C�Y
	int m_graphWidth;
	int m_graphHeight;
};