#include "Enemy1.h"
#include "DxLib.h"
#include <cassert>
#include "Player.h"
#include "Camera.h"
#include "Map.h"

#ifdef _DEBUG
#define DISP_COLLISION
#endif

namespace
{
	// グラフィックのサイズ
	constexpr int kGraphWidth  = 100;
	constexpr int kGraphHeight = 100;

	// 初期位置
	constexpr float kEnemyDefaultPosX = 500;
	constexpr float kEnemyDefaultPosY = 4443;

	// 重力
	constexpr float kGravity = 0.4f;

	// 各アニメーションのコマ数
	constexpr int kWalkAnimNum = 8;

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 8;

	// グラフィックの拡大率
	constexpr float kExpRate = 3.5f;

	// 移動速度
	constexpr float kSpeed = 2.5f;

	// ノックバック距離
	constexpr int kBesideHit   = 43;

	// ダメージ食らった後の無敵時間
	constexpr int kDamageBlinkFrame = 30;
}

Enemy1::Enemy1(Camera& camera) :
	m_handleRun(-1),
	m_isDirLeft(false),
	m_blinkFrameCount(0),
	EnemyBase(Vec3(kEnemyDefaultPosX, kEnemyDefaultPosY), camera)
{
	m_animAllFrame = 0;
}

Enemy1::~Enemy1()
{
}

void Enemy1::Init()
{
	m_handleRun = LoadGraph("img/Enemy/Orc/OrcWalk.png");
	assert(m_handleRun != -1);

	m_idleRun.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExpRate, kWalkAnimNum);

	m_pos.x = kEnemyDefaultPosX;
	m_pos.y = kEnemyDefaultPosY;

	m_hp = 3;
}

void Enemy1::Update()
{
}

void Enemy1::Draw()
{
}

void Enemy1::Update(Map& map)
{
	m_idleRun.Update();

	// 移動処理
	if (!m_isDirLeft)
	{
		m_vec.x = kSpeed;
	}
	else if (m_isDirLeft)
	{
		m_vec.x = -kSpeed;
	}

	m_pos.x += m_vec.x;
	// 横から当たっているかどうかを確認する
	Rect chipRect;
	if (map.IsCol(GetRect(), chipRect, m_camera))
	{
		// 左右どっちから当たったか

		// 右方向に移動している
		if (m_vec.x > 0.0f)
		{
			// 右壁に当たっているので左に押し戻す
			m_isDirLeft = true;
			m_pos.x = chipRect.left - kBesideHit;
		}
		// 左方向に移動している
		else if (m_vec.x < 0.0f)
		{
			// 左壁に当たっているので右に押し戻す
			m_isDirLeft = false;
			m_pos.x = chipRect.right + kBesideHit + 30;
		}
	}

	// 死亡
	if (m_hp <= 0)
	{
		DeleteGraph(m_handleRun);
	}
}

void Enemy1::Draw(Camera& camera)
{
	// 点滅処理
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	//Vec3 drawPos = m_camera.Capture(m_pos);

	Vec3 camOffset = camera.GetDrawOffset();
	camOffset.x = 0;

#ifdef DISP_COLLISION
	if (m_hp > 0)
	{
		DrawBox(GetLeft(), GetTop() + camOffset.y, GetRight(), GetBottom() + camOffset.y, 0xff0000, false);
	}
	m_idleRun.Play(m_pos + camOffset, m_isDirLeft);
#endif

	/*DrawFormatString(0, 30, 0xffffff, "EnemyPos.X~%f, Y=%f", m_pos.x, m_pos.y);
	DrawFormatString(0, 45, 0xffffff, "drawPos.X~%f, Y=%f", drawPos.x, drawPos.y);*/
}

void Enemy1::End()
{
	DeleteGraph(m_handleRun);
}

float Enemy1::GetLeft()
{
	return (m_pos.x - 20);
}

float Enemy1::GetRight()
{
	return (m_pos.x + 25);
}

float Enemy1::GetTop()
{
	return (m_pos.y - 35);
}

float Enemy1::GetBottom()
{
	return (m_pos.y + 35);
}

Rect Enemy1::GetRect()
{
	// プレイヤーの矩形当たり判定を作成
	Rect rect;
	rect.top    = GetTop();
	rect.bottom = GetBottom();
	rect.right  = GetRight();
	rect.left   = GetLeft();
	return rect;
}

void Enemy1::OnDamage()
{
	// 既にダメージを受けている(無敵時間は)
	// 再度ダメージを受けることは無い
	if (m_blinkFrameCount > 0) return;

	// 無敵時間(点滅する時間)を設定する
	m_blinkFrameCount = kDamageBlinkFrame;

	// ダメージを受ける
	m_hp--;
}
