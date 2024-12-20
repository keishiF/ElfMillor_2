#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"
#include "Boss.h"
#include "Enemy1.h"
#include "Map.h"
#include "Camera.h"

#include <cassert>

namespace
{
	// 初期位置
	constexpr int kDefaultPlayerPosX = 360;
	constexpr int kDefaultPlayerPosY = 400;

	// 画面端
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	constexpr int kFieldHeight = 352;

	// プレイヤーの初期HP
	constexpr int kDefaultHp = 15;

	// プレイヤーの移動速度
	constexpr float kSpeed = 5.0f;

	// 重力
	constexpr float kGravity = 0.4f;
	constexpr float kZeroGravity = 0;

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 8;

	// キャラクターのグラフィックのサイズ
	constexpr int kGraphWidth = 160;
	constexpr int kGraphHeight = 128;

	// 当たり判定のサイズ
	constexpr int kColSizeWidth = 0;
	constexpr int kColSizeHeight = 0;

	// 待機アニメーションのコマ数
	constexpr int kIdleAnimNum = 8;
	// 走りアニメーションのコマ数
	constexpr int kRunAnimNum = 8;
	// 攻撃アニメーションのコマ数
	constexpr int kAtkAnimNum = 13;
	// 死亡アニメーションのコマ数
	constexpr int kDeathAnimNum = 10;

	// グラフィックの拡大率
	constexpr float kExpRate = 2.0f;

	// ノックバック距離
	constexpr int kKnockBack = 60;

	// 
}

Player::Player(Camera& camera) :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleAtk(-1),
	m_handleDeath(-1),
	m_isRun(false),
	m_isJump(false),
	m_isAtk(false),
	m_isDeath(false),
	m_jumpSpeed(-10.0f),
	m_jumpCount(0),
	m_vec(0.0f,0.0f),
	m_isDirLeft(false),
	m_isShotDirRight(true),
	m_blinkFrame(0),
	m_hp(kDefaultHp),
	m_isGroundHit(false),
	m_isCeilingHit(false),
	m_isLastJump(false),
	m_isLastJumpButton(false),
	m_shot(),
	m_idleAnim(),
	m_runAnim(),
	m_atkAnim(),
	// 基底クラスの初期化
	GameObject(Vec3(kDefaultPlayerPosX, kDefaultPlayerPosY), camera)
{
}

Player::~Player()
{
	// グラフィックの解放
	DeleteGraph(m_handleIdle);
	DeleteGraph(m_handleRun);
	DeleteGraph(m_handleAtk);
	DeleteGraph(m_handleDeath);
}

void Player::Init()
{
	// グラフィックの読み込み
	m_handleIdle = LoadGraph("img/Player2/Idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("img/Player2/Run.png");
	assert(m_handleRun != -1);

	m_handleAtk = LoadGraph("img/Player2/Atk.png");
	assert(m_handleAtk != -1);

	m_handleDeath = LoadGraph("img/Player2/Death.png");
	assert(m_handleDeath != -1);

	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Init();
	}

	m_hp = kDefaultHp;

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kIdleAnimNum);
	m_runAnim.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kRunAnimNum);
	m_atkAnim.Init(m_handleAtk, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kAtkAnimNum);
}

void Player::End()
{	
}

void Player::Update(Input& input, Boss& boss, Enemy1& enemy1, Map& map)
{
	m_idleAnim.Update();

	m_isRun = false;
	m_vec.x = 0.0f;

	if (m_isRun)
	{
		m_runAnim.Update();
	}
	if (m_isAtk)
	{
		m_atkAnim.Update();
	}

	// 左走り
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_vec.x -= kSpeed;
		m_isShotDirRight = false;
	}
	// 右走り
	else if (input.IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_vec.x += kSpeed;
		m_isShotDirRight = true;
	}

	// ジャンプ
	// 空中にいるときの処理
	if (m_isJump)
	{
		m_vec.y += kGravity;

		m_pos.x += m_vec.x;
		Rect chipRect;
		if (map.IsCol(GetRect(), chipRect))
		{
			if (m_vec.x > 0.0f)
			{
				m_pos.x = chipRect.left - kGraphWidth * 0.5f - 1;
			}
			else if (m_vec.x < 0.0f)
			{
				m_pos.x = chipRect.right + kGraphWidth * 0.5f + 1;
			}
		}

		m_pos.y += m_vec.y;
		if (map.IsCol(GetRect(), chipRect))
		{
			if (m_vec.y > 0.0f)
			{
				m_pos.y -= m_vec.y;
				m_vec.y = 0.0f;
				m_isJump = false;
			}
			else if (m_vec.y < 0.0f)
			{
				m_pos.y = chipRect.bottom + kGraphHeight * 0.5f + 1;
				m_vec.y *= -1.0f;
			}
		}
	}
	else
	{
		if (input.IsTrigger(PAD_INPUT_1) && !m_isJump)
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

		Rect chipRect;
		if (map.IsCol(GetRect(), chipRect))
		{
			if (m_vec.x > 0.0f)
			{
				m_pos.x = chipRect.left - kGraphWidth - 1;
			}
			else if (m_vec.x < 0.0f)
			{
				m_pos.x = chipRect.right + kGraphWidth + 1;
			}
		}

		m_pos.y += m_vec.y;

		if (map.IsCol(GetRect(), chipRect))
		{
			if (m_vec.y > 0.0f)
			{
				m_pos.y = chipRect.top - 1;
				m_isJump = false;
			}
			else if (m_vec.y < 0.0f)
			{
				m_pos.y = chipRect.bottom + kGraphHeight + 1;
				m_vec.y *= -1.0f;
			}
		}
		else
		{
			m_isJump = true;
		}
	}

	// 前フレームでジャンプしていたかを確認
	if (input.IsTrigger(PAD_INPUT_1))
	{
		m_isLastJumpButton = true;
	}
	else
	{
		m_isLastJumpButton = false;
	}

	// 攻撃
	if (input.IsTrigger(PAD_INPUT_2))
	{
		for (int i = 0; i < kShot; ++i)
		{
			if (!m_shot[i].m_isShotFlag)
			{
				// アニメーション切り替え
				m_isAtk = true;

				// 弾の位置をプレイヤーの位置に補正
				m_shot[i].m_pos = m_pos;

				// 弾を表示
				m_shot[i].m_isShotFlag = true;

				// 弾の向きをプレイヤーと同じ向きに補正
				m_shot[i].m_isDirLeft = m_isShotDirRight;

				// 弾を1発出してループから抜ける
				break;
			}
		}
	}
	else
	{
		m_isAtk = false;
	}

	//// 右端に行ったら左端に
	//if (m_pos.x <= kLeftEndWidth)
	//{
	//	m_pos.x = kRightEndWidth;
	//}
	//// 左端に行ったら右端に
	//else if (m_pos.x >= kRightEndWidth)
	//{
	//	m_pos.x = kLeftEndWidth;
	//}

	// 被弾
	if (enemy1.m_hp > 0)
	{
		if (GetRight() > enemy1.GetLeft() &&
			GetLeft() < enemy1.GetRight() &&
			GetTop() < enemy1.GetBottom() &&
			GetBottom() > enemy1.GetTop())
		{
			// HPを減らす
			//m_hp--;

			// 当たった相手がいない方向にノックバック
			if (m_pos.x < enemy1.m_pos.x)
			{
				m_vec.x -= kKnockBack;
			}
			else if (m_pos.x > enemy1.m_pos.x)
			{
				m_vec.x += kKnockBack;
			}
		}
	}

	// HPがなくなったら死ぬ
	if (m_hp <= 0)
	{
		DeleteGraph(m_handleIdle);
		DeleteGraph(m_handleRun);
		DeleteGraph(m_handleAtk);
		DeleteGraph(m_handleDeath);
	}

	// 弾を発射
	for (int i = 0; i < kShot; i++)
	{	
		m_shot[i].Update(boss, enemy1);
	}
}

void Player::Draw()
{
	//Vec3 drawPos = m_camera.Capture(m_pos);

	if (m_hp >= 0)
	{
		DrawBox(GetLeft(), GetTop(), GetRight(), GetBottom(), 0xff0000, true);
	}
	if (m_isRun)
	{
		m_runAnim.Play(m_pos, m_isDirLeft);
	}
	else if (m_isAtk)
	{
		m_atkAnim.Play(m_pos, m_isDirLeft);
	}
	else
	{
		m_idleAnim.Play(m_pos, m_isDirLeft);
	}
	for (int i = 0; i < kShot; i++)
	{
		m_shot[i].Draw();
	}

	DrawFormatString(0, 0, 0xffffff, "PlayerPos.X=%f,Y=%f", m_pos.x, m_pos.y);
	//DrawFormatString(0, 15, 0xffffff, "DrawPos.X=%f,Y=%f", drawPos.x, m_pos.y);
}

float Player::GetLeft()
{
	return (m_pos.x - 50);
}

float Player::GetRight()
{
	return (m_pos.x + 50);
}

float Player::GetTop()
{
	return (m_pos.y - 25);
}

float Player::GetBottom()
{
	return (m_pos.y + kGraphHeight - 20);
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