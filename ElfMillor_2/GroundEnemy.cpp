#include "GroundEnemy.h"
#include "Player.h"
#include "Camera.h"
#include "Map.h"

#include "DxLib.h"
#include <cassert>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

namespace
{
	// グラフィックのサイズ
	constexpr int kGraphWidth  = 100;
	constexpr int kGraphHeight = 100;

	// 画面端
	constexpr int kLeftEndWidth = 160;
	constexpr int kRightEndWidth = 1120;

	// 初期HP
	constexpr int kDefaultHp = 3;

	// 重力
	constexpr float kGravity = 0.4f;

	// 各アニメーションのコマ数
	constexpr int kWalkAnimNum = 8;

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 8;

	// グラフィックの拡大率
	constexpr float kExtRate = 3.5f;

	// グラフィックの回転率
	constexpr float kRotaRate = 0.0f;

	// 移動速度
	constexpr float kSpeed = 2.5f;

	// ノックバック距離
	constexpr int kBesideHit   = 43;

	// ダメージ食らった後の無敵時間
	constexpr int kDamageBlinkFrame = 30;
}

GroundEnemy::GroundEnemy(std::weak_ptr<Camera> camera):
	m_handleRun(-1),
	m_isDirLeft(false),
	m_blinkFrameCount(0),
	EnemyBase(Vec3(0.0f, 0.0f), camera)
{
}

GroundEnemy::~GroundEnemy()
{
}

void GroundEnemy::Init(float posX, float posY)
{
	m_handleRun = LoadGraph("img/Enemy/Orc/OrcWalk.png");
	assert(m_handleRun != -1);

	m_runAnim.Init(m_handleRun, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kWalkAnimNum);

	m_hp = kDefaultHp;

	m_pos.x = posX;
	m_pos.y = posY;
}

void GroundEnemy::Update(Map& map)
{
	// 無敵時間の更新
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}

	m_runAnim.Update();

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

	// 右端に行ったら左端に
	if (m_pos.x <= kLeftEndWidth)
	{
		m_pos.x = kRightEndWidth;
	}
	// 左端に行ったら右端に
	else if (m_pos.x >= kRightEndWidth)
	{
		m_pos.x = kLeftEndWidth;
	}

	// 死亡
	if (m_hp <= 0)
	{
		DeleteGraph(m_handleRun);
	}
}

void GroundEnemy::Draw()
{
	// 点滅処理
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	//Vec3 drawPos = m_camera.Capture(m_pos);

	Vec3 camOffset = m_camera.lock()->GetDrawOffset();
	camOffset.x = 0;

#ifdef DISP_COLLISION
	if (m_hp > 0)
	{
		DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop() + camOffset.y),
			static_cast<int>(GetRight()), static_cast<int>(GetBottom() + camOffset.y), 0xff0000, false);
	}
#endif

	m_runAnim.Play(m_pos + camOffset, m_isDirLeft);

	/*DrawFormatString(0, 30, 0xffffff, "EnemyPos.X~%f, Y=%f", m_pos.x, m_pos.y);
	DrawFormatString(0, 45, 0xffffff, "drawPos.X~%f, Y=%f", drawPos.x, drawPos.y);*/
}

void GroundEnemy::End()
{
	DeleteGraph(m_handleRun);
}

float GroundEnemy::GetLeft()
{
	return (m_pos.x - 20);
}

float GroundEnemy::GetRight()
{
	return (m_pos.x + 25);
}

float GroundEnemy::GetTop()
{
	return (m_pos.y - 35);
}

float GroundEnemy::GetBottom()
{
	return (m_pos.y + 35);
}

Rect GroundEnemy::GetRect()
{
	// プレイヤーの矩形当たり判定を作成
	Rect rect;
	rect.top    = GetTop();
	rect.bottom = GetBottom();
	rect.right  = GetRight();
	rect.left   = GetLeft();
	return rect;
}

void GroundEnemy::OnDamage()
{
	// 既にダメージを受けている(無敵時間は)
	// 再度ダメージを受けることは無い
	if (m_blinkFrameCount > 0) return;

	// 無敵時間(点滅する時間)を設定する
	m_blinkFrameCount = kDamageBlinkFrame;

	// ダメージを受ける
	m_hp--;
}
