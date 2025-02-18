#pragma once
#include "EnemyBase.h"
#include "Rect.h"
#include "Animation.h"

class Player;
class Camera;
class Map;

class GroundEnemy :public EnemyBase
{
public:
	GroundEnemy(std::weak_ptr<Camera> camera);
	~GroundEnemy();

	// �������A�X�V�A�`��
	void Init(float posX, float posY) override;
	void Update(Player& player, Map& map) override;
	void Draw() override;

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

	// Enemy�̍��W���擾
	Vec3 GetPos() { return m_pos; }

private:
	// ��e���̓_��
	void UpdateBlinkFrame();
	int  m_blinkFrameCount;

	// �O���t�B�b�N�n���h��
	int  m_handleRun;
	int  m_handleDead;

	// SE�n���h��
	int  m_seHandle;

	// ���������ǂ���
	bool m_isDirLeft;

	// ����ł��邩�ǂ���
	bool m_isDead;
	// ���S�A�j���[�V����
	Animation m_deadAnim;
};