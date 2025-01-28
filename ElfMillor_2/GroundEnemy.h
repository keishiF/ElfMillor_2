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
	void End();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect  GetRect();

	void OnDamage();

	// Enemyの座標を取得
	Vec3 GetPos() { return m_pos; }

private:
	void UpdateBlinkFrame();

	// グラフィックハンドル
	int  m_handleRun;
	int  m_handleDead;

	// 左向きかどうか
	bool m_isDirLeft;

	// 被弾時の点滅
	int m_blinkFrameCount;

	// アニメーション
	Animation m_deadAnim;
};