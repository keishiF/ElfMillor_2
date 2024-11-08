#include "Player.h"
#include "DxLib.h"
#include <cassert>
#include "Pad.h"

namespace
{
	// プレイヤーの初期HP
	constexpr int kMaxHp = 5;

	// プレイヤーの移動速度
	constexpr int kSpeed = 0.2f;

	// 各アニメーションのコマ数
	constexpr int kIdleAnimFrame = 11;

	// アニメーション1コマのフレーム数
	constexpr int kSingleAnimFrame = 5;

	// キャラクターのグラフィックのサイズ
	constexpr int kGraphWidth = 80;
	constexpr int kGraphHeight = 80;

	// 各アニメーションのフレーム数
	constexpr int kIdleAnimNum = 9;
	constexpr int kRunAnimNum = 5;
}

Player::Player() :
	m_handleIdle(-1),
	m_handleRun(-1),
	m_animFrame(0),
	m_isRun(false),
	m_isJump(false),
	m_blinkFrame(0),
	m_hp(kMaxHp),
	m_deadFrame(0),
	m_isLastJump(false)
{
}

Player::~Player()
{
}

void Player::Init()
{
	// グラフィックの読み込み
	m_handleIdle = LoadGraph("img/Player/Idle.png");
	assert(m_handleIdle != -1);

	m_handleRun = LoadGraph("img/Player/Run.png");
	assert(m_handleIdle != -1);
}

void Player::End()
{
	// グラフィックの解放
	DeleteGraph(m_handleIdle);
}

void Player::Update()
{
	// アニメーションの更新
	m_animFrame++;

	int totalFrame = kIdleAnimNum * kSingleAnimFrame;
	if (m_isRun)
	{
		totalFrame = kRunAnimNum * kSingleAnimFrame;
	}

	if (m_animFrame >= totalFrame)
	{
		m_animFrame = 0;
	}

	m_isRun = false;
	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_isRun = true;
	}
	else if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		m_isRun = true;
	}
	else
	{
		m_isRun = false;
	}
}

void Player::Draw()
{
	int animNo = m_animFrame / kSingleAnimFrame;

	// 仮表示位置
	int playerPosX = 460;
	int playerPosY = 345;

	int useHandle = m_handleIdle;
	if (m_isRun)
	{
		useHandle = m_handleRun;
	}


	DrawRectRotaGraph(playerPosX, playerPosY,
		animNo * kGraphWidth, 0, kGraphWidth, kGraphHeight,
		2.0f, 0.0f,
		useHandle, true, false);
}