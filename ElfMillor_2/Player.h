#pragma once
#include "Vec3.h"

class Input;

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
	/*int m_handleJump;
	int m_handleAtk;
	int m_handleUpAtk;*/

	// アニメーションのフレーム数を数える
	int m_animFrame;

	bool m_isRun;

	// プレイヤーの表示位置
	//Vec3 m_pos;

	// ジャンプ
	bool m_isJump;

	// 被弾時の点滅
	int m_blinkFrame;

	// プレイヤーのHP
	int m_hp;

	// 死亡演出用のフレーム
	int m_deadFrame;

	// 無限ジャンプしないように管理する
	bool m_isLastJump;
};