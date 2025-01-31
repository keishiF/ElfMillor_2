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

	int GetHp() { return m_hp; }

private:
	void UpdateBlinkFrame();

	int  m_handleFly;
	int  m_handleDead;
	int  m_seHandle;

	bool m_isDirLeft;

	// ”í’e‚Ì“_–Å
	int m_blinkFrameCount;

	bool m_isDead;
	Animation m_deadAnim;

	// ˆÚ“®•ûŒü
	int m_moveDir;

	// ‰ŠúˆÊ’u‚ğ•Û‘¶‚·‚é
	float m_initPosY;
};