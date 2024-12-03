#include "Shot.h"
#include "Player.h"
#include "DxLib.h"
#include "game.h"
#include "Vec3.h"
#include "Boss.h"
#include "Enemy1.h"

#include <cmath>
#include <cassert>

namespace
{
	// ’e‚ÌƒOƒ‰ƒtƒBƒbƒNƒTƒCƒY
	constexpr int kGraphWidth = 68;
	constexpr int kGraphHeight = 9;

	// ’e‚ÌˆÚ“®§ŒÀ
	constexpr int kBulletRifhtX = 1120;
	constexpr int kBulletLeftX = 160;

	// ’e‚Ì”¼Œa
	constexpr float kShotRadius = 5.0f;
}

Shot::Shot():
	m_isShotFlag(false),
	m_handle(-1),
	m_isDirLeft(true),
	m_pos(0,0),
	m_velocity(5.0f,0.0f),
	m_graphWidth(kGraphWidth),
	m_graphHeight(kGraphHeight),
	m_isUp(false)
{
}

Shot::~Shot()
{
	DeleteGraph(m_handle);
}

void Shot::Init()
{
	m_handle = LoadGraph("img/Bullet/Bullet.png");
	assert(m_handle != -1);
}

void Shot::Update(Boss& boss, Enemy1& enemy1)
{
	if (m_isUp)
	{
		m_pos.y--;
	}
	else if (m_isDirLeft)
	{
		m_pos += m_velocity;
	}
	else if (!m_isDirLeft)
	{
		m_pos -= m_velocity;
	}

	if (m_pos.x > 1120)
	{
		m_isShotFlag = false;
	}
	if (m_pos.x < 160)
	{
		m_isShotFlag = false;
	}

	if (boss.m_hp > 0)
	{
		if (5 + 100 > std::abs(m_pos.x - boss.m_pos.x) &&
			5 + 100 > std::abs(m_pos.y - boss.m_pos.y))
		{
			if (m_isShotFlag)
			{
				m_isShotFlag = false;
				boss.m_hp--;
			}
		}
	}

	if (enemy1.m_hp > 0)
	{
		if (GetRight() > enemy1.GetLeft() &&
			GetLeft() < enemy1.GetRight() &&
			GetTop() < enemy1.GetBottom() &&
			GetBottom() > enemy1.GetTop())
		{
			if (m_isShotFlag)
			{
				m_isShotFlag = false;
				enemy1.m_hp--;
			}
		}
	}
}

void Shot::Draw()
{
	if (m_isShotFlag)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), static_cast<int>(kShotRadius), 0xff0000, true);
		//DrawRectRotaGraph(m_pos.x, m_pos.y, 0, 0, kGraphWidth, kGraphHeight, 2.0f, 0.0f, m_handle, true, m_isDirLeft);
	}
}

float Shot::GetLeft()
{
	return m_pos.x - kShotRadius;
}

float Shot::GetRight()
{
	return m_pos.x + kShotRadius;
}

float Shot::GetTop()
{
	return m_pos.y - kShotRadius;
}

float Shot::GetBottom()
{
	return m_pos.y + kShotRadius;
}