#include "Shot.h"
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

	// エフェクトのグラフィックサイズ
	constexpr int kEffectGraphWidth = 64;
	constexpr int kEffectGraphHeight = 64;

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 4;

	// エフェクトのアニメーションのコマ数
	constexpr int kEffectAnimNum = 14;

	// グラフィックの拡大率
	constexpr float kExtRate = 2.0f;

	// グラフィックの回転率
	constexpr float kRotaRate = 0.0f;

	// 画面端
	constexpr int kLeftEndWidth  = 160;
	constexpr int kRightEndWidth = 1120;

	// 弾の半径
	constexpr int kShotRadius = 15;

	// 弾の速度
	constexpr float kShotSpeed = 5.0f;
}

Shot::Shot() :
	m_isShot(false),
	m_isEffect(false),
	m_shotHandle(-1),
	m_effectHandle(-1),
	m_isDirLeft(true),
	m_pos(0,0),
	m_velocity(kShotSpeed,kShotSpeed),
	m_effectAnim()
{
}

Shot::~Shot()
{
	DeleteGraph(m_shotHandle);
	DeleteGraph(m_effectHandle);
}

void Shot::Init()
{
	m_shotHandle = LoadGraph("data/image/Bullet/Bullet.png");
	assert(m_shotHandle != -1);

	m_effectHandle = LoadGraph("data/image/Effect/effect.png");
	assert(m_effectHandle != -1);

	m_effectAnim.Init(m_effectHandle, kAnimSingleFrame, kEffectGraphWidth, kEffectGraphHeight, kExtRate, kRotaRate, kEffectAnimNum);
}

void Shot::Update(std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
	std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, std::weak_ptr<Camera> camera, Map& map)
{
	// 弾が出たときに上入力されているかどうか
	if (m_isUp)
	{
		// されていたら横の力をなくして上にしか進まないようにする
		m_velocity = { 0.0f, kShotSpeed };
		m_pos -= m_velocity;
	}
	else
	{
		// されていなかったら縦の力をなくして横にしか進まないようにする
		m_velocity = { kShotSpeed, 0.0f };
		// 左向きかどうか
		if (!m_isDirLeft)
		{
			// 左向きじゃないので右に進む
			m_pos += m_velocity;
		}
		else if (m_isDirLeft)
		{
			// 左向きなので左に進む
			m_pos -= m_velocity;
		}
	}

	// 右端に行ったら左端に
	if (m_pos.x <= kLeftEndWidth)
	{
		m_isShot = false;
	}
	// 左端に行ったら右端に
	else if (m_pos.x >= kRightEndWidth)
	{
		m_isShot = false;
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
				if (m_isShot)
				{
					m_isShot  = false;
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
				if (m_isShot)
				{
					m_isShot = false;
					flyingEnemy[i]->OnDamage();
				}
			}
		}
	}

	Rect chipRect;
	if (map.IsCol(GetRect(), chipRect, camera))
	{
		if (m_isShot)
		{
			m_isShot = false;
		}
	}
}

void Shot::Draw(std::weak_ptr<Camera> camera)
{
	Vec3 camOffset = camera.lock()->GetDrawOffset();
	camOffset.x    = 0;

	if (m_isShot)
	{
		if (m_isUp)
		{
			DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y + camOffset.y),
				0, 0, kGraphWidth, kGraphHeight, 1.0f, -42.5f, m_shotHandle, true, m_isDirLeft);
		}
		else
		{
			DrawRectRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y + camOffset.y),
				0, 0, kGraphWidth, kGraphHeight, 1.0f, 0.0f, m_shotHandle, true, m_isDirLeft);
		}
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