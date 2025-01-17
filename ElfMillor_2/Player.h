#pragma once
#include "Vec3.h"
#include "Animation.h"
#include "Shot.h"
#include "GameObject.h"
#include "Rect.h"

class Boss;
class Enemy1;
class Map;
class Camera;

namespace
{
	constexpr int kShot = 5;
}

class Player : public GameObject
{
public:
	// プレイヤーのコンストラクタとデストラクタ
	Player(Camera& camera);
	~Player();

	// プレイヤーの初期化、終了処理、更新処理、描画処理
	void Init();
	void End();
	void Draw(Camera& camera);
	void Update(Input& input, Boss& boss, Enemy1& enemy1, Map& map, Camera& camera);

	// 状態ごとのアップデート
	void NormalUpdate(Input& input, Boss& boss, Enemy1& enemy1, Map& map, Camera& camera);
	void DeadUpdate(Camera& camera);
	
	// ダメージ処理
	void OnDamage();

	// 当たり判定のための座標取得
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	// プレイヤーの矩形情報取得
	Rect GetRect();

	// プレイヤーの表示位置
	Vec3 m_vec;

private:
	// プレイヤーのグラフィックハンドル
	int m_handleIdle;
	int m_handleRun;
	int m_handleAtk;
	int m_handleDead;

	// 各アニメーションを切り替えるフラグ
	bool m_isRun;
	bool m_isJump;
	bool m_isAtk;
	bool m_isDead;

	// ジャンプスピード
	float m_jumpSpeed;
	int   m_jumpCount;

	// プレイヤーが左向きかどうか
	bool m_isDirLeft;

	// 地面に当たっているかどうかのフラグ
	bool m_isGroundHit;

	// 天井との当たり判定
	bool m_isCeilingHit;

	// 被弾時の点滅
	int m_blinkFrameCount;

	// 死亡時のフレーム
	int m_deadFrameCount;

	// プレイヤーのHP
	int m_hp;

	// 無限ジャンプしないように管理する
	bool m_isLastJump;
	bool m_isLastJumpButton;

	// 弾の情報
	Shot m_shot[kShot];

	// 各アニメーション
	Animation m_idleAnim;
	Animation m_runAnim;
	Animation m_atkAnim;
	Animation m_deadAnim;
};