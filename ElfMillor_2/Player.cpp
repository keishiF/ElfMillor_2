#include "Player.h"
#include "Input.h"
#include "DxLib.h"
#include "game.h"

#include <cassert>

namespace
{
	// 画面中央
	constexpr int kGameScreenHalfWidth = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;

	// 画面端
	constexpr int kLeftEndWidth = 0;
	constexpr int kRightEndWidth = 1280;

	// プレイヤーの初期HP
	constexpr int kMaxHp = 5;

	// プレイヤーの移動速度
	constexpr int kSpeed = 5.0f;

	// 各アニメーションのコマ数
	constexpr int kIdleAnimFrame = 11;

	// アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 8;

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
	m_isAtk(false),
	m_isDeath(false),
	m_pos(kGameScreenHalfWidth,kGameScreenHalfHeight),
	m_isDirLeft(false),
	m_blinkFrame(0),
	m_hp(kMaxHp),
	m_isLastJump(false)
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
}

void Player::End()
{	
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
		totalFrame = kAtkAnimNum * kSingleAnimFrame;
	}
	if (m_isDeath)
	{
		totalFrame = kDeathAnimNum * kSingleAnimFrame;
	}

	if (m_animFrame >= totalFrame)
	{
		m_animFrame = 0;
	}

	// 走り
	m_isRun = false;
	// 左走り
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
		m_isDirLeft = true;
		m_pos.x -= kSpeed;
	}
	// 右走り
	else if (input.IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
		m_isDirLeft = false;
		m_pos.x += kSpeed;
	}
	// 走ってない
	else
	{
		m_isRun = false;
	}
	// 攻撃
	if (input.IsTrigger(PAD_INPUT_2))
	{
		m_isAtk = true;
	}
	// 攻撃していない
	else
	{
		m_isAtk = false;
	}
	// 死亡
	if (input.IsPress(PAD_INPUT_4))
	{
		m_isDeath = true;
	}
	// 生きている
	else
	{
		m_isDeath = false;
	}

	if (m_pos.x <= kLeftEndWidth)
	{
		m_pos.x = kRightEndWidth;
	}
	else if (m_pos.x >= kRightEndWidth)
	{
		m_pos.x = kLeftEndWidth;
	}
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