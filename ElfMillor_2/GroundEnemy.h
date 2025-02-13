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

	// 初期化、更新、描画
	void Init(float posX, float posY) override;
	void Update(Player& player, Map& map) override;
	void Draw() override;

	// 矩形情報
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();
	Rect  GetRect();

	// ダメージ処理
	void OnDamage();

	// HPの取得
	int GetHp() { return m_hp; }

	// Enemyの座標を取得
	Vec3 GetPos() { return m_pos; }

private:
	// 被弾時の点滅
	void UpdateBlinkFrame();
	int  m_blinkFrameCount;

	// グラフィックハンドル
	int  m_handleRun;
	int  m_handleDead;

	// SEハンドル
	int  m_seHandle;

	// 左向きかどうか
	bool m_isDirLeft;

	// 死んでいるかどうか
	bool m_isDead;
	// 死亡アニメーション
	Animation m_deadAnim;
};