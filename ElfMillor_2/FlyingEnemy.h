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

	void Init(float posX, float posY) override;
	void Update(Player& player, Map& map) override;
	void Draw() override;
	void End();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect  GetRect();

	void OnDamage();

private:
	void UpdateBlinkFrame();

	int  m_handleFly;

	bool m_isDirLeft;

	// ��e���̓_��
	int m_blinkFrameCount;

	// �ړ�����
	int m_moveDir;

	// �����ʒu��ۑ�����
	float m_initPosY;
};