#include "Bullet.h"
#include "Player.h"
#include "DxLib.h"
#include "game.h"

#include <cassert>

namespace
{
	// 弾のグラフィックサイズ
	constexpr int kGraphWidth = 68;
	constexpr int kGraphHeight = 9;

	// 弾の移動制限
	constexpr int kBulletRifhtX = 1120;
	constexpr int kBulletLeftX = 160;
}

Bullet::Bullet():
	m_isShotFlag(false),
	m_handle(-1),
	m_isDirLeft(true),
	m_pos(0,0),
	m_graphWidth(kGraphWidth),
	m_graphHeight(kGraphHeight)
{
}

Bullet::~Bullet()
{
	DeleteGraph(m_handle);
}

void Bullet::Init()
{
	m_handle = LoadGraph("img/Bullet/Bullet.png");
	assert(m_handle != -1);
}

void Bullet::Draw()
{
	if (m_isShotFlag)
	{
		if (m_isDirLeft)
		{
			m_pos.x += 5;
		}
		else if (!m_isDirLeft)
		{
			m_pos.x -= 5;
		}

		if (m_pos.x > Game::kScreenWidth)
		{
			m_isShotFlag = false;
		}
		if (m_pos.x < 0)
		{
			m_isShotFlag = false;
		}

		DrawRectRotaGraph(m_pos.x, m_pos.y, 0, 0, kGraphWidth, kGraphHeight, 2.0f, 0.0f, m_handle, true, m_isDirLeft);
	}
}

float Bullet::GetLeft()
{
	return m_pos.x - kGraphWidth * 0.5f;
}

float Bullet::GetRight()
{
	return m_pos.x + kGraphWidth * 0.5f;
}

float Bullet::GetTop()
{
	return m_pos.y - kGraphHeight * 0.5f;
}

float Bullet::GetBottom()
{
	return m_pos.y + kGraphHeight * 0.5f;
}