#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"
#include "Boss.h"
#include "Enemy1.h"
#include "Map.h"

#include <cassert>

namespace
{
	// 初期位置
	constexpr int kPlayerPosX = 0; // 270
	constexpr int kPlayerPosY = 0; // 460

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

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 8;

	// キャラクターのグラフィックのサイズ
	constexpr int kGraphWidth = 160;
	constexpr int kGraphHeight = 128;

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
	constexpr int kKnockBack = 30;
}

Player::Player() :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleAtk(-1),
	m_handleDeath(-1),
	m_isRun(false),
	m_isJump(false),
	m_isAtk(false),
	m_isDeath(false),
	m_jumpSpeed(0.0f),
	m_pos(kPlayerPosX, kPlayerPosY),
	m_vec(),
	m_isDirLeft(false),
	m_isShotDirRight(true),
	m_isUp(false),
	m_blinkFrame(0),
	m_hp(kDefaultHp),
	m_isLastJump(false),
	m_isLastJumpButton(false),
	m_shot(),
	m_idleAnim(),
	m_runAnim(),
	m_atkAnim()
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
	// 走ってない
	else
	{
		m_isRun = false;
		m_vec.x = 0;
	}

	// ジャンプ
	if (input.IsTrigger(PAD_INPUT_1))
	{
		if (!m_isJump)
		{
			m_isJump = true;
			m_jumpSpeed = -10.0f;
		}
	}
	if (m_isJump)
	{
		m_pos.y += m_jumpSpeed;
		m_jumpSpeed += kGravity;

		if (m_jumpSpeed > 0.0f)
		{
			if (m_pos.y >= kPlayerPosY)
			{
				m_isJump = false;
				m_jumpSpeed = 0.0f;
			}
		}
	}
	if (input.IsTrigger(PAD_INPUT_1))
	{
		m_isLastJumpButton = true;
	}
	else
	{
		m_isLastJumpButton = false;
	}

	// 移動処理
	m_vec = m_vec.GetNormalize() * kSpeed;
	m_pos += m_vec;

	// 上入力しているかどうか
	if (input.IsTrigger(PAD_INPUT_UP))
	{
		m_isUp = true;
	}
	else
	{
		m_isUp = false;
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

				// 弾に上入力しているかどうかを教える
				m_shot[i].m_isUp = m_isUp;

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
			m_hp--;

			// 当たった相手がいない方向にノックバック
			if (m_pos.x < enemy1.m_pos.x)
			{
				m_pos.x -= kKnockBack;
				enemy1.m_pos.x += kKnockBack;
			}
			else if (m_pos.x > enemy1.m_pos.x)
			{
				m_pos.x += kKnockBack;
				enemy1.m_pos.x -= kKnockBack;
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

	// マップとの当たり判定
	for (int y = 0; y < kMapHeight; y++)
	{
		for (int x = 0; x < kMapWidth; x++)
		{
			for (int i = 0; i < _countof(kWhiteList); i++)
			{
				if (map.mapChips[y][x].chipNo == kWhiteList[i])
				{
					MapChip chip = map.mapChips[y][x];
					float chipBottom = chip.m_pos.y + kMapChipSize;
					float chipTop = chip.m_pos.y;
					float chipRight = chip.m_pos.x + kMapChipSize;
					float chipLeft = chip.m_pos.x;
					if (GetTop() < chipBottom ||
						GetBottom() > chipTop ||
						GetRight() > chipLeft ||
						GetLeft() < chipRight)
					{
						//printfDx("当たってる\n");
					}
					else
					{
						//printfDx("当たってない\n");
					}
				}
			}
		}
	}
}

void Player::Draw()
{
	DrawBox(static_cast<int>(m_pos.x - kGraphWidth * 0.5f), static_cast<int>(m_pos.y), static_cast<int>(m_pos.x + kGraphWidth * 0.5f), static_cast<int>(m_pos.y + kGraphHeight), 0x0000ff, false);
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
}

float Player::GetLeft()
{
	return (m_pos.x - kGraphWidth * 0.5f);;
}

float Player::GetRight()
{
	return (m_pos.x + kGraphWidth * 0.5f);
}

float Player::GetTop()
{
	return (m_pos.y - kGraphHeight);
}

float Player::GetBottom()
{
	return (m_pos.y + kGraphHeight * 0.5f);
}