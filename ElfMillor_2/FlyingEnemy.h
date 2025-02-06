#pragma once
#include "EnemyBase.h"
#include "Rect.h"
#include "Animation.h"

class Player;
class Camera;
class Map;

class FlyingEnemy : public EnemyBase
{
public:
	FlyingEnemy(std::weak_ptr<Camera> camera);
	~FlyingEnemy();

	// �������A�X�V�A�`��
	void Init(float posX, float posY) override;
	void Update(Player& player, Map& map) override;
	void Draw() override;
	void End();

	// ��`���
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();
	Rect  GetRect();

	// �_���[�W����
	void OnDamage();

	// HP�̎擾
	int GetHp() { return m_hp; }

private:
	// ��e���̓_��
	void UpdateBlinkFrame();
	int m_blinkFrameCount;

	// �G�̊e�n���h��
	int  m_handleFly;
	int  m_handleDead;
	int  m_seHandle;

	// ���������ǂ����̃t���O
	bool m_isDirLeft;

	// ����ł��邩�ǂ���
	bool m_isDead;
	// ���S�A�j���[�V����
	Animation m_deadAnim;

	// �ړ�����
	int m_moveDir;

	// �����ʒu��ۑ�����
	float m_initPosY;
};