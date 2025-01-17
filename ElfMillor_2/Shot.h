#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Animation.h"
#include "GameObject.h"

class Input;
class Boss;
class Enemy1;
class Camera;
class Map;

class Shot
{
public:
	Shot();
	~Shot();

	void Init();
	void Update(Boss& boss, Enemy1& enemy1, Camera& camera, Map& map);
	void Draw(Camera& camera);

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect GetRect();

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

	// �A�j���[�V����
	Animation m_shotAnim;
};