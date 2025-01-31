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

	void Init(float posX, float posY) override;
	void Update(Player& player, Map& map) override;
	void Draw() override;

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect  GetRect();

	void OnDamage();

	int GetHp() { return m_hp; }

	// Enemyの座標を取得
	Vec3 GetPos() { return m_pos; }

private:
	void UpdateBlinkFrame();

	// グラフィックハンドル
	int  m_handleRun;
	int  m_handleDead;

	// SEハンドル
	int  m_seHandle;

	// 左向きかどうか
	bool m_isDirLeft;

	bool m_isDead;

	// 被弾時の点滅
	int m_blinkFrameCount;

	Animation m_deadAnim;
};