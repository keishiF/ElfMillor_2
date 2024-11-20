#include "Boss.h"
#include "DxLib.h"

#include <cassert>

namespace
{
	// グラフィックのサイズ
	constexpr int kGraphWidth = 224;
	constexpr int kGraphHeight = 240;

	// 各アニメーションのコマ数
	constexpr int kIdleAnimNum = 15;

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 8;

	// グラフィックの拡大率
	constexpr float kExpRate = 2.0f;

	// 初期位置
	constexpr float kBossPosX = 700;
	constexpr float kBossPosY = 330;
}

Boss::Boss()
{
	m_animAllFrame = 0;
}

Boss::~Boss()
{
}

void Boss::Init()
{	
	m_handleIdle = LoadGraph("img/Boss/Boss.png");
	assert(m_handleIdle != -1);

	m_idleAnim.Init(m_handleIdle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kIdleAnimNum);
	m_pos.x = kBossPosX;
	m_pos.y = kBossPosY;
}

void Boss::Update()
{
	m_idleAnim.Update();
}

void Boss::Draw()
{
	m_isDirLeft = false;
	m_idleAnim.Play(m_pos, m_isDirLeft);
}