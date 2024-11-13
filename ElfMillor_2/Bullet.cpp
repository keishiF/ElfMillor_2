#include "Bullet.h"
#include "Player.h"

#include <DxLib.h>
#include <cassert>

namespace
{
	// 弾のグラフィックサイズ
	constexpr int kGraphWidth = 0;
	constexpr int kGraphHeight = 0;
}

Bullet::Bullet():
	m_isShotFlag(false),
	m_handle(-1),
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

void Bullet::Draw(Bullet& bullet)
{
	if (m_isShotFlag)
	{
		bullet.m_pos.x += 5;
	}

	if (bullet.m_pos.x > 1280)
	{
		m_isShotFlag = false;
	}
	DrawGraph(bullet.m_pos.x, bullet.m_pos.y, m_handle, false);
}