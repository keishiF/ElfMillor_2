#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"

#include <cassert>

namespace
{
	// 初期位置
	constexpr int kPlayerPosX = 170;
	constexpr int kPlayerPosY = 388;

	// 画面端
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	constexpr int kFieldHeight = 352;

	// プレイヤーの初期HP
	constexpr int kMaxHp = 5;

	// プレイヤーの移動速度
	constexpr int kSpeed = 5.0f;

	// 重力
	constexpr float kGravity = 0.4f;

	// 各アニメーションのコマ数
	constexpr int kIdleAnimFrame = 11;

	// アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 8;
	constexpr int kAtkAnimFrame = 8;

	// キャラクターのグラフィックのサイズ
	constexpr int kGraphWidth = 160;
	constexpr int kGraphHeight = 128;

	// 待機アニメーションのフレーム数
	constexpr int kIdleAnimNum = 8;
	// 走りアニメーションのフレーム数
	constexpr int kRunAnimNum = 8;
	// 攻撃アニメーションのフレーム数
	constexpr int kAtkAnimNum = 13;
	// 死亡アニメーションのフレーム数
	constexpr int kDeathAnimNum = 10;
}

Player::Player() :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_handleAtk(-1),
	m_handleDeath(-1),
	m_animFrame(0),
	m_isRun(false),
	m_isJump(false),
	m_isAtk(false),
	m_isDeath(false),
	m_jumpSpeed(0.0f),
	m_pos(kPlayerPosX, kPlayerPosY),
	m_vec(),
	m_isDirLeft(false),
	m_isBulletDirRight(true),
	m_blinkFrame(0),
	m_hp(kMaxHp),
	m_isLastJump(false),
	m_isLastJumpButton(false)
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

	m_pBullet = new Bullet;
	m_pBullet->Init();
}

void Player::End()
{	
	delete m_pBullet;
}

void Player::Update(Input& input)
{
	// アニメーションの更新
	m_animFrame++;
	int totalFrame = kIdleAnimNum * kSingleAnimFrame;
	if (m_isRun)
	{
		totalFrame = kRunAnimNum * kSingleAnimFrame;
	}
	if (m_isAtk)
	{
		totalFrame = kAtkAnimNum * kAtkAnimFrame;
	}
	if (m_isDeath)
	{
		totalFrame = kDeathAnimNum * kSingleAnimFrame;
	}

	if (m_animFrame >= totalFrame)
	{
		m_animFrame = 0;
		if (m_isAtk)
		{
			m_isAtk = false;
		}
	}

	// 走り
	m_isRun = false;
	// 左走り
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_vec.x -= kSpeed;
		m_isBulletDirRight = false;
	}
	// 右走り
	else if (input.IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_vec.x += kSpeed;
		m_isBulletDirRight = true;
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

	if (input.IsTrigger(PAD_INPUT_2))
	{
		m_isAtk = true;
		m_pBullet->m_pos = m_pos;
		m_pBullet->m_isShotFlag = true;
		m_pBullet->m_isDirLeft = m_isBulletDirRight;
	}

	//// 死亡
	//if (input.IsPress(PAD_INPUT_4))
	//{
	//	m_isDeath = true;
	//}
	//// 生きている
	//else
	//{
	//	m_isDeath = false;
	//}

	if (m_pos.x <= kLeftEndWidth)
	{
		m_pos.x = kRightEndWidth;
	}
	else if (m_pos.x >= kRightEndWidth)
	{
		m_pos.x = kLeftEndWidth;
	}

	m_pBullet->Draw();
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;

	// 表示するグラフィックをプレイヤーの状態に応じて変更
	// 何もしていないときに待機アニメーション
	int useHandle = m_handleIdle;

	// 走り
	if (m_isRun)
	{
		useHandle = m_handleRun;
	}
	// 攻撃
	if (m_isAtk)
	{
		useHandle = m_handleAtk;
	}
	// 死亡
	if (m_isDeath)
	{
		useHandle = m_handleDeath;
	}

	// 描画
	DrawRectRotaGraph(m_pos.x, m_pos.y,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0f, 0.0f,
		useHandle, true, m_isDirLeft);
}