#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Animation.h"
#include "GameObject.h"

#include <vector>
#include <memory>

class Input;
class Boss;
class GroundEnemy;
class FlyingEnemy;
class Camera;
class Map;

class Shot
{
public:
	Shot();
	~Shot();

	void Init();
	void Update(std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
		std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, std::weak_ptr<Camera> camera, Map& map);
	void Draw(std::weak_ptr<Camera> camera);

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect GetRect();

	// �e���o�Ă��邩�ǂ����̃t���O
	bool m_isShot;

	// �G�t�F�N�g���o�邩�ǂ���
	bool m_isEffect;

	// �e�̈ʒu���W
	Vec3 m_pos;
	Vec3 m_velocity;

	// ���������ǂ���
	bool m_isDirLeft;

	// ����͂���Ă��邩�̃t���O
	bool m_isUp;

private:
	// �e�̃O���t�B�b�N�n���h��
	int m_shotHandle;

	// �e��������Ƃ��̃G�t�F�N�g�n���h��
	int m_effectHandle;

	// �G�t�F�N�g�̃A�j���[�V����
	Animation m_effectAnim;
};