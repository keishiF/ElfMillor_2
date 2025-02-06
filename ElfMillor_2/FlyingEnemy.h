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

	// 初期化、更新、描画
	void Init(float posX, float posY) override;
	void Update(Player& player, Map& map) override;
	void Draw() override;
	void End();

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

private:
	// 被弾時の点滅
	void UpdateBlinkFrame();
	int m_blinkFrameCount;

	// 敵の各ハンドル
	int  m_handleFly;
	int  m_handleDead;
	int  m_seHandle;

	// 左向きかどうかのフラグ
	bool m_isDirLeft;

	// 死んでいるかどうか
	bool m_isDead;
	// 死亡アニメーション
	Animation m_deadAnim;

	// 移動方向
	int m_moveDir;

	// 初期位置を保存する
	float m_initPosY;
};