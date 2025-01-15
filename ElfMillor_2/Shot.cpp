#include "Shot.h"
#include "Player.h"
#include "DxLib.h"
#include "game.h"
#include "Vec3.h"
#include "Boss.h"
#include "Enemy1.h"
#include "Camera.h"
#include "Map.h"

#include <cmath>
#include <cassert>

namespace
{
	// 弾のグラフィックサイズ
	constexpr int kGraphWidth   = 68;
	constexpr int kGraphHeight  = 9;

	// 画面端
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	// 弾の半径
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

void Shot::Update(Boss& boss, Enemy1& enemy1, Camera& camera, Map& map)
{
	if (m_isDirLeft)
	{
		m_pos += m_velocity;
	}
	else if (!m_isDirLeft)
	{
		m_pos -= m_velocity;
	}

	// 右端に行ったら左端に
	if (m_pos.x <= kLeftEndWidth)
	{
		m_isShotFlag = false;
	}
	// 左端に行ったら右端に
	else if (m_pos.x >= kRightEndWidth)
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

	Rect chipRect;
	if (map.IsCol(GetRect(), chipRect, camera))
	{
		if (m_isShotFlag)
		{
			m_isShotFlag = false;
		}
	}
}

void Shot::Draw(Camera& camera)
{
	Vec3 camOffset = camera.GetDrawOffset();
	camOffset.x = 0;

	if (m_isShotFlag)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y) + camOffset.y, static_cast<int>(kShotRadius), 0xff0000, true);
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

Rect Shot::GetRect()
{
	Rect rect;
	rect.top = GetTop();
	rect.bottom = GetBottom();
	rect.right = GetRight();
	rect.left = GetLeft();
	return rect;
}