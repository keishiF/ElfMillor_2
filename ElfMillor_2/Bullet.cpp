#include "Bullet.h"
#include "Player.h"

#include <DxLib.h>
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
	m_isDirLeft(false),
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
		m_pos.x += 5;
		/*if (m_pos.x > 1120)
		{
			m_isShotFlag = false;
		}*/
		DrawRectRotaGraph(m_pos.x, m_pos.y, 0, 0, kGraphWidth, kGraphHeight, 2.0f, 0.0f, m_handle, true,m_isDirLeft);
	}
}