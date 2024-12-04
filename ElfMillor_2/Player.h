#pragma once
#include "Vec3.h"
#include "Animation.h"
#include "Shot.h"

class Vec3;
class Boss;
class Enemy1;
class Map;

namespace
{
	constexpr int kShot = 15;
}

class Player
{
public:
	// プレイヤーのコンストラクタとデストラクタ
	Player();
	~Player();

	// プレイヤーの初期化、終了処理、更新処理、描画処理
	void Init();
	void End();
	void Update(Input& input, Boss& boss, Enemy1& enemy1, Map& map);
	void Draw();

	// 当たり判定のための座標取得
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	// プレイヤーの表示位置
	Vec3 m_pos;
	Vec3 m_vec;

private:
	// プレイヤーのグラフィックハンドル
	int m_handleIdle;
	int m_handleRun;
	int m_handleAtk;
	int m_handleDeath;

	// 各アニメーションを切り替えるフラグ
	bool m_isRun;
	bool m_isJump;
	bool m_isAtk;
	bool m_isDeath;

	// ジャンプスピード
	float m_jumpSpeed;

	// プレイヤーが左向きかどうか
	bool m_isDirLeft;
	// 弾を右向きにするかどうか
	bool m_isShotDirRight;

	// 上入力されているかどうか
	bool m_isUp;

	// 被弾時の点滅
	int m_blinkFrame;

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
};