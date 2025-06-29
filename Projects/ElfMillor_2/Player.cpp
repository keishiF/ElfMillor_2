#include "Player.h"
#include "Camera.h"
#include "Map.h"

#include "game.h"
#include "Input.h"

#include "DxLib.h"
#include <cassert>
#include <iostream>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

namespace
{
	// 初期位置
	constexpr int kPlayerInitPosX = 360;
	constexpr int kPlayerInitPosY = 7600;

	// 画面端
	constexpr int kLeftScreenEdge = 160;
	constexpr int kRightScreenEdge = 1120;

	// プレイヤーの初期HP
	constexpr int kDefaultHp = 7;

	// プレイヤーの移動速度
	constexpr float kSpeed = 5.0f;

	// 重力
	constexpr float kGravity = 0.4f;

	// アニメーション1コマのフレーム数
	constexpr int kPlayerAnimSingleFrame = 8;
	constexpr int kEffectAnimSingleFrame = 8;

	// キャラクターのグラフィックサイズ
	constexpr int kPlayerGraphWidth = 160;
	constexpr int kPlayerGraphHeight = 128;

	// 死亡時のエフェクトのグラフィックサイズ
	constexpr int kEffectGraphWidth = 48;
	constexpr int kEffectGraphHeight = 48;

	// 待機アニメーションのコマ数
	constexpr int kIdleAnimNum = 8;
	// 走りアニメーションのコマ数
	constexpr int kRunAnimNum = 8;
	// 攻撃アニメーションのコマ数
	constexpr int kAtkAnimNum = 13;
	// 死亡アニメーションのコマ数
	constexpr int kDeadAnimNum = 18;

	// グラフィックの拡大率
	constexpr float kPlayerExtRate = 1.75f;
	constexpr float kEffectExtRate = 15.0f;

	// グラフィックの回転率
	constexpr float kRotaRate = 0.0f;

	// ノックバック距離
	constexpr int kBesideHit = 20;
	constexpr int kVerticalHit = 1;

	// ダメージ食らった後の無敵時間
	constexpr int kDamageBlinkFrame = 45;

	// 死亡時の演出再生時間
	constexpr int kDeadFrame = 80;

	// ダメージを受けた際に減少するスコア
	constexpr int kSubScore = 500;
}

Player::Player(std::weak_ptr<Camera> camera) :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleDead(-1),
	m_shotSEHandle(-1),
	m_loopSEHandle(-1),
	m_isRun(false),
	m_isJump(false),
	m_isDead(false),
	m_jumpSpeed(-10.5f),
	m_jumpCount(0),
	m_isDirLeft(false),
	m_isUp(false),
	m_blinkFrameCount(0),
	m_deadFrameCount(0),
	m_hp(kDefaultHp),
	m_isLastJumpButton(false),
	m_isClear(false),
	m_shot(),
	m_idleAnim(),
	m_runAnim(),
	m_deadAnim(),
	// 基底クラスの初期化
	GameObject(Vec3(kPlayerInitPosX, kPlayerInitPosY), camera)
{
}

Player::~Player()
{
	// グラフィックの解放
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleDead);

	// SEの解放
	DeleteSoundMem(m_shotSEHandle);
	DeleteSoundMem(m_loopSEHandle);
}

void Player::Init()
{
	// グラフィックの読み込み
	m_handleIdle = LoadGraph("data/image/Player/Idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("data/image/Player/Run.png");
	assert(m_handleRun != -1);

	m_handleDead = LoadGraph("data/image/Effect/effect2.png");
	assert(m_handleDead != -1);

	// SEの読み込み
	m_shotSEHandle = LoadSoundMem("data/sound/SE/shotSE.mp3");
	assert(m_shotSEHandle != -1);

	m_loopSEHandle = LoadSoundMem("data/sound/SE/loopSE.mp3");
	assert(m_loopSEHandle != -1);

	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Init();
	}

	m_idleAnim.Init(m_handleIdle, kPlayerAnimSingleFrame, kPlayerGraphWidth, kPlayerGraphHeight, kPlayerExtRate, kRotaRate, kIdleAnimNum);
	m_runAnim.Init(m_handleRun, kPlayerAnimSingleFrame, kPlayerGraphWidth, kPlayerGraphHeight, kPlayerExtRate, kRotaRate, kRunAnimNum);
	m_deadAnim.Init(m_handleDead, kEffectAnimSingleFrame, kEffectGraphWidth, kEffectGraphHeight, kEffectExtRate, kRotaRate, kDeadAnimNum);
}

void Player::Update(Input& input, std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
	std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, Map& map)
{
	if (!m_isDead)
	{
		// 無敵時間の更新
		UpdateBlinkFrame();

		m_idleAnim.Update();

		m_isRun = false;
		m_vec.x = 0.0f;

		if (m_isRun)
		{
			m_runAnim.Update();
		}

		HandleMovement(input, map);

		// 前フレームでジャンプしていたかを確認
		m_isLastJumpButton = input.IsTrigger("B");

		// 上入力
		if (input.IsPress("UP"))
		{
			m_isUp = true;
		}
		else
		{
			m_isUp = false;
		}

		// 攻撃
		HandleAttack(input);

		// 右端に行ったら左端に
		if (m_pos.x <= kLeftScreenEdge)
		{
			PlaySoundMem(m_loopSEHandle, DX_PLAYTYPE_BACK, true);
			m_pos.x = kRightScreenEdge;
		}
		// 左端に行ったら右端に
		else if (m_pos.x >= kRightScreenEdge)
		{
			PlaySoundMem(m_loopSEHandle, DX_PLAYTYPE_BACK, true);
			m_pos.x = kLeftScreenEdge;
		}

		// 弾を発射
		for (int i = 0; i < kShot; i++)
		{
			m_shot[i].Update(groundEnemy, flyingEnemy, m_camera, map);
		}
	}

	if (m_hp <= 0)
	{
		m_isDead = true;
	}
}

void Player::Draw()
{
	// 点滅処理
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	Vec3 camOffset = m_camera.lock()->GetDrawOffset();
	camOffset.x = 0;

	// プレイヤーのアニメーション切り替え
	// 死んだ
	if (m_isDead)
	{
		m_deadAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
	// 走る
	else if (m_isRun)
	{
		m_runAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
	// 待機
	else
	{
		m_idleAnim.Play(m_pos + camOffset, m_isDirLeft);
	}

#ifdef DISP_COLLISION
	// プレイヤーの当たり判定の表示
	if (m_hp >= 0)
	{
		DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop() + camOffset.y),
			static_cast<int>(GetRight()), static_cast<int>(GetBottom() + camOffset.y), 0xff0000, false);
	//	DrawCircle(static_cast<int>(m_pos.x) + camOffset.x,
	//		static_cast<int>(m_pos.y) + camOffset.y, 10, 0xff00ff, false);
		DrawFormatString(5, 30, 0xffffff, "PlayerPos.X=%f,Y=%f", m_pos.x, m_pos.y);
	}
#endif

	// ショット
	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Draw(m_camera);
	}

	//DrawFormatString(5, 0, 0xffffff, "Hp = %d", m_hp);
}

void Player::OnDamage()
{
#ifdef DISP_COLLISION
	printfDx("ダメージ！\n");
#endif

	// 既にダメージを受けている(無敵時間は)
	// 再度ダメージを受けることは無い
	if (m_blinkFrameCount > 0) return;

	// 無敵時間(点滅する時間)を設定する
	m_blinkFrameCount = kDamageBlinkFrame;

	// ダメージを受ける
	m_hp--;

	if (GetScoreManager().GetScore() > 0)
	{
		GetScoreManager().SubScore(kSubScore);
	}
}

float Player::GetLeft()
{
	return (m_pos.x - 15);
}

float Player::GetRight()
{
	return (m_pos.x + 15);
}

float Player::GetTop()
{
	return (m_pos.y +5);
}

float Player::GetBottom()
{
	return (m_pos.y + kPlayerGraphHeight - 35);
}

Rect Player::GetRect()
{
	// プレイヤーの矩形当たり判定を作成
	Rect rect;
	rect.top = GetTop();
	rect.bottom = GetBottom();
	rect.right = GetRight();
	rect.left = GetLeft();
	return rect;
}

void Player::UpdateBlinkFrame()
{
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}
}

void Player::HandleMovement(Input& input, Map& map)
{
	// 左走り
	if (input.IsPress("LEFT"))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_vec.x -= kSpeed;
	}
	// 右走り
	else if (input.IsPress("RIGHT"))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_vec.x += kSpeed;
	}

	// ジャンプ
	if (m_isJump)
	{
		HandleJump(map);
	}
	else
	{
		HandleGroundMovement(input, map);
	}
}

void Player::HandleJump(Map& map)
{
	// 空中にいるときの処理

	// 毎フレーム重力によって下方向に加速する
	m_vec.y += kGravity;

	// 移動処理
	m_pos.x += m_vec.x;

	// 横から当たったかどうかを確認する
	Rect chipRect;
	if (map.IsCol(GetRect(), chipRect, m_camera))
	{
		// 左右どっちから当たったか

		// プレイヤーが右方向に移動している
		if (m_vec.x > 0.0f)
		{
			// 右壁に当たっているので左に押し戻す
			m_pos.x = chipRect.left - kBesideHit;
		}
		// プレイヤーが左方向に移動している
		else if (m_vec.x < 0.0f)
		{
			// 左壁に当たっているので右に押し戻す
			m_pos.x = chipRect.right + kBesideHit;
		}
	}

	m_pos.y += m_vec.y;

	// 縦から当たったかどうかを確認する
	if (map.IsCol(GetRect(), chipRect, m_camera))
	{
		// 上下どっちから当たったか

		// プレイヤーが下方向に移動している
		if (m_vec.y > 0.0f)
		{
			m_isJump = false;
			// 床に当たっているので上に押し戻す
			m_pos.y -= m_vec.y;
			m_vec.y = 0.0f;
		}
		// プレイヤーが上方向に移動している
		else if (m_vec.y < 0.0f)
		{
			// 天井に当たっているので下に押し戻す
			m_pos.y = chipRect.bottom + kVerticalHit;
			m_vec.y = 0.0f;
		}
	}

	// 下からはすり抜け、上からは乗れる床の判定
	if (map.IsCol2(GetRect(), chipRect, m_camera))
	{
		// 上下どっちから当たったか

		// プレイヤーが下方向に移動している
		if (m_vec.y > 0.0f)
		{
			// 前フレームのプレイヤーの位置
			Vec3 posTemp = m_pos - m_vec;
			// プレイヤーの頂点から床の中心までの距離
			float playerMapDistance = chipRect.GetCenter().y - posTemp.y;
			// 判定を取ってほしい距離
			float distance = MapConsts::kMapChipSize * 0.5f + GetBottom() - GetTop() - 5;
			// プレイヤーの頂点から床の中心までの距離が判定を取ってほしい距離より大きい
			if (playerMapDistance > distance)
			{
				m_isJump = false;
				// 床に当たっているので上に押し戻す
				m_pos.y -= m_vec.y;
				m_vec.y = 0.0f;
			}
		}
	}

	// ダメージ床との当たり判定
	if (map.IsDamageCol(GetRect(), chipRect, m_camera))
	{
		OnDamage();
	}

	// クリアのフラグとなっている鍵との当たり判定
	if (map.IsClearCol(GetRect(), chipRect, m_camera))
	{
		m_isClear = true;
	}
}

void Player::HandleGroundMovement(Input& input, Map& map)
{
	// ジャンプ
	if (input.IsPress("B") && !m_isJump)
	{
		m_isJump = true;
		m_jumpCount++;
	}
	else
	{
		m_jumpCount = 0;
	}

	if (m_isJump && m_jumpCount == 1)
	{
		m_vec.y = m_jumpSpeed;
	}
	else
	{
		m_isJump = false;
	}

	// 移動処理
	m_pos.x += m_vec.x;

	// 横から当たっているかどうかを確認する
	Rect chipRect;
	if (map.IsCol(GetRect(), chipRect, m_camera))
	{
		// 左右どっちから当たったか

		// プレイヤーが右方向に移動している
		if (m_vec.x > 0.0f)
		{
			// 右壁に当たっているので左に押し戻す
			m_pos.x = chipRect.left - kBesideHit;
		}
		// プレイヤーが左方向に移動している
		else if (m_vec.x < 0.0f)
		{
			// 左壁に当たっているので右に押し戻す
			m_pos.x = chipRect.right + kBesideHit;
		}
	}

	m_pos.y += m_vec.y;

	// 縦から当たっているかどうかを確認する
	if (map.IsCol(GetRect(), chipRect, m_camera))
	{
		// 上下どっちから当たったか

		// プレイヤーが下方向に移動している
		if (m_vec.y > 0.0f)
		{
			m_isJump = false;
			// 床に当たっているのでその高さに合わせる
			m_pos.y = chipRect.top;
		}
		// プレイヤーが上方向に移動している
		else if (m_vec.y < 0.0f)
		{
			// 天井に当たっているので下に押し戻す
			m_pos.y = chipRect.bottom + kVerticalHit;
			m_vec.y = 0.0f;
		}
	}
	else
	{
		m_isJump = true;
	}

	// 下からはすり抜け、上からは乗れる床の判定
	if (map.IsCol2(GetRect(), chipRect, m_camera))
	{
		// 上下どっちから当たったか

		// プレイヤーが下方向に移動している
		if (m_vec.y > 0.0f)
		{
			// 前フレームのプレイヤーの位置
			Vec3 posTemp = m_pos - m_vec;
			// プレイヤーの頂点から床の中心までの距離
			float playerMapDistance = chipRect.GetCenter().y - posTemp.y;
			// 判定を取ってほしい距離
			float distance = MapConsts::kMapChipSize * 0.5f + GetBottom() - GetTop() - 5;
			// プレイヤーの頂点から床の中心までの距離が判定を取ってほしい距離より大きい
			if (playerMapDistance > distance)
			{
				m_isJump = false;
				// 床に当たっているのでその高さに合わせる
				m_pos.y = chipRect.top;
			}
		}
	}
	else
	{
		m_isJump = true;
	}

	// ダメージ床との当たり判定
	if (map.IsDamageCol(GetRect(), chipRect, m_camera))
	{
		OnDamage();
	}

	// クリアのフラグとなっている鍵との当たり判定
	if (map.IsClearCol(GetRect(), chipRect, m_camera))
	{
		m_isClear = true;
	}
}

void Player::HandleAttack(Input& input)
{
	if (input.IsTrigger("A"))
	{
		for (int i = 0; i < kShot; ++i)
		{
			if (!m_shot[i].m_isShot)
			{
				// ショット時の音の再生
				PlaySoundMem(m_shotSEHandle, DX_PLAYTYPE_BACK, true);

				// 弾の位置をプレイヤーの位置に補正
				m_shot[i].m_pos.x = m_pos.x;
				m_shot[i].m_pos.y = m_pos.y + 35;

				// 弾を表示
				m_shot[i].m_isShot = true;

				// 弾の向きをプレイヤーと同じ向きに補正
				m_shot[i].m_isDirLeft = m_isDirLeft;

				// 上入力されているかどうかをプレイヤーと同じ状態にする
				m_shot[i].m_isUp = m_isUp;

				// 弾を1発出してループから抜ける
				break;
			}
		}
	}
}