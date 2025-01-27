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

	// 被弾時の点滅
	int m_blinkFrameCount;

	// 移動方向
	int m_moveDir;

	// 初期位置を保存する
	float m_initPosY;
};