#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Shot.h"
#include "Input.h"
#include "Boss.h"
#include "GroundEnemy.h"
#include "FlyingEnemy.h"
#include "Map.h"
#include <memory>
#include <vector>

namespace
{
	constexpr int kShot = 5;
}

class Player : public GameObject
{
public:
    Player(std::weak_ptr<Camera> camera);
    ~Player();

    // プレイヤーの初期化、終了処理
    void Init();
    void End();

    // プレイヤーの更新、描画処理
    void Update(Input& input, Boss& boss, std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
        std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, Map& map);
    void Draw();

    // 死亡時の更新処理
    void DeadUpdate();

    // ダメージを受けた時の処理
    void OnDamage();

	// プレイヤーのHPを取得
	int GetHp() { return m_hp; }

    // プレイヤーの矩形当たり判定を取得
    float GetLeft();
    float GetRight();
    float GetTop();
    float GetBottom();
    Rect GetRect();

    // プレイヤーの座標を取得
    Vec3 GetPos() { return m_pos; }

private:
    // 無敵時間の更新
    void UpdateBlinkFrame();

    // 移動処理
    void HandleMovement(Input& input, Map& map);
    void HandleJump(Map& map);
    void HandleGroundMovement(Input& input, Map& map);

    // 攻撃処理
    void HandleAttack(Input& input);

    // プレイヤーのグラフィックハンドル
    int m_handleIdle;
    int m_handleRun;
    int m_handleAtk;
    int m_handleDead;

    // アニメーションフラグ
    bool m_isRun;
    bool m_isJump;
    bool m_isAtk;
    bool m_isDead;

    // ジャンプ関連
    float m_jumpSpeed;
    int m_jumpCount;

    // プレイヤーの向き
    bool m_isDirLeft;

    // 無敵時間関連
    int m_blinkFrameCount;
    int m_deadFrameCount;

    // プレイヤーのHP
    int m_hp;

    // 地面、天井との当たり判定フラグ
    bool m_isGroundHit;
    bool m_isCeilingHit;

    // ジャンプボタンの状態
    bool m_isLastJump;
    bool m_isLastJumpButton;

    // ショット
    Shot m_shot[kShot];

    // アニメーション
    Animation m_idleAnim;
    Animation m_runAnim;
    Animation m_atkAnim;
    Animation m_deadAnim;
};