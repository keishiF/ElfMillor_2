#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Shot.h"
#include "Input.h"
#include "GroundEnemy.h"
#include "FlyingEnemy.h"
#include "Map.h"
#include "ScoreManager.h"
#include <memory>
#include <vector>

namespace
{
    // ショットの数
	constexpr int kShot = 5;
}

class Player : public GameObject
{
public:
    Player(std::weak_ptr<Camera> camera);
    ~Player();

    // 初期化、更新、描画
    void Init();
    void Update(Input& input, std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
        std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, Map& map);
    void Draw();

    // ダメージを受けた時の処理
    void OnDamage();

	// プレイヤーのHPを取得
	int GetHp() { return m_hp; }

    // プレイヤーの矩形当たり判定を取得
    float GetLeft();
    float GetRight();
    float GetTop();
    float GetBottom();
    Rect  GetRect();

    // プレイヤーの座標を取得
    Vec3 GetPos() { return m_pos; }

    ScoreManager& GetScoreManager() { return m_scoreManager; }

    // クリアフラグ
    bool m_isClear;

    // 死亡アニメーション
    Animation m_deadAnim;

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
    int m_handleDead;

    // SEハンドル
    int m_shotSEHandle;
    int m_loopSEHandle;

    // アニメーションフラグ
    bool m_isRun;
    bool m_isJump;
    bool m_isDead;

    // ジャンプ関連
    float m_jumpSpeed;
    int   m_jumpCount;

    // プレイヤーの向き
    bool m_isDirLeft;

    // プレイヤーが上入力をしているかどうか
    bool m_isUp;

    // 無敵時間関連
    int m_blinkFrameCount;
    int m_deadFrameCount;

    // プレイヤーのHP
    int m_hp;

    // ジャンプボタンの状態
    bool m_isLastJumpButton;

    ScoreManager m_scoreManager;

    // ショット
    Shot m_shot[kShot];

    // アニメーション
    Animation m_idleAnim;
    Animation m_runAnim;
};