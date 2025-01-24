#include "Shot.h"
#include "Boss.h"
#include "GroundEnemy.h"
#include "FlyingEnemy.h"
#include "Camera.h"
#include "Map.h"

#include "game.h"

#include "DxLib.h"
#include <cmath>
#include <cassert>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

namespace
{
	// 弾のグラフィックサイズ
	constexpr int kGraphWidth   = 127;
	constexpr int kGraphHeight  = 123;
	//constexpr int kGraphWidth = 10;
	//constexpr int kGraphHeight = 26;

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 8;

	// アニメーションのコマ数
	constexpr int kShotAnimNum = 10;

	// グラフィックの拡大率
	constexpr float kExtRate = 2.0f;

	// グラフィックの回転率
	constexpr float kRotaRate = 0.0f;

	// 画面端
	constexpr int kLeftEndWidth  = 160;
	constexpr int kRightEndWidth = 1120;

	// 弾の半径
	constexpr int kShotRadius = 15;
}

Shot::Shot():
	m_isShotFlag(false),
	m_handle(-1),
	m_isDirLeft(true),
	m_pos(0,0),
	m_velocity(5.0f,0.0f),
	m_isUp(false),
	m_shotAnim()
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

	m_shotAnim.Init(m_handle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kShotAnimNum);
}

void Shot::Update(Boss& boss, std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
	std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, std::weak_ptr<Camera> camera, Map& map)
{
	m_shotAnim.Update();

	if (!m_isDirLeft)
	{
		m_pos += m_velocity;
	}
	else if (m_isDirLeft)
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

	for (int i = 0; i < groundEnemy.size(); i++)
	{
		if (groundEnemy[i]->m_hp > 0)
		{
			if (GetRight()  > groundEnemy[i]->GetLeft()   &&
				GetLeft()   < groundEnemy[i]->GetRight()  &&
				GetTop()    < groundEnemy[i]->GetBottom() &&
				GetBottom() > groundEnemy[i]->GetTop())
			{
				if (m_isShotFlag)
				{
					m_isShotFlag  = false;
					groundEnemy[i]->OnDamage();
				}
			}
		}
	}

	for (int i = 0; i < flyingEnemy.size(); i++)
	{
		if (flyingEnemy[i]->m_hp > 0)
		{
			if (GetRight() > flyingEnemy[i]->GetLeft() &&
				GetLeft() < flyingEnemy[i]->GetRight() &&
				GetTop() < flyingEnemy[i]->GetBottom() &&
				GetBottom() > flyingEnemy[i]->GetTop())
			{
				if (m_isShotFlag)
				{
					m_isShotFlag = false;
					flyingEnemy[i]->OnDamage();
				}
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

void Shot::Draw(std::weak_ptr<Camera> camera)
{
	Vec3 camOffset = camera.lock()->GetDrawOffset();
	camOffset.x    = 0;

	if (m_isShotFlag)
	{
		DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y + camOffset.y),
			0, 0, kGraphWidth, kGraphHeight, 1.0f, 0.0f, m_handle, true, m_isDirLeft);
		//m_shotAnim.Play(m_pos + camOffset, m_isDirLeft);
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
	rect.top    = GetTop();
	rect.bottom = GetBottom();
	rect.right  = GetRight();
	rect.left   = GetLeft();
	return rect;
}