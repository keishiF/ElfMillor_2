#pragma once
#include "Vec3.h"
#include "Bullet.h"
#include "Animation.h"
#include "DxLib.h"

class Input;
class Vec3;

namespace
{
	constexpr int kShot = 3;
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
	void Update(Input& input);
	void Draw();

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

	// プレイヤーの表示位置
	Vec3 m_pos;
	Vec3 m_vec;

	// プレイヤーが左向きかどうか
	bool m_isDirLeft;
	// 弾を右向きにするかどうか
	bool m_isBulletDirRight;

	// 被弾時の点滅
	int m_blinkFrame;

	// プレイヤーのHP
	int m_hp;

	// 無限ジャンプしないように管理する
	bool m_isLastJump;
	bool m_isLastJumpButton;

	// 弾の情報
	Bullet m_bullet[kShot];

	// 各アニメーション
	Animation m_idleAnim;
	Animation m_runAnim;
	Animation m_atkAnim;
};