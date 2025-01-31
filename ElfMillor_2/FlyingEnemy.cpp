#include "FlyingEnemy.h"
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
	// 敵のグラフィックのサイズ
	constexpr int kGraphWidth = 150;
	constexpr int kGraphHeight = 120;

	// 死亡時のエフェクトのグラフィックサイズ
	constexpr int kEffectGraphWidth = 48;
	constexpr int kEffectGraphHeight = 48;

	// 初期HP
	constexpr int kDefaultHp = 3;

	// 重力
	constexpr float kGravity = 0.4f;

	// 各アニメーションのコマ数
	constexpr int kWalkAnimNum = 8;
	constexpr int kDeadAnimNum = 18;

	// アニメーション1コマのフレーム数
	constexpr int kAnimSingleFrame = 8;
	constexpr int kDeadAnimSingleFrame = 3;

	// グラフィックの拡大率
	constexpr float kExtRate = 2.5f;
	constexpr float kEffectExtRate = 4.5f;

	// グラフィックの回転率
	constexpr float kRotaRate = 0.0f;

	// 移動速度
	constexpr float kSpeed = 2.5f;

	// ノックバック距離
	constexpr int kBesideHit = 43;

	// ダメージ食らった後の無敵時間
	constexpr int kDamageBlinkFrame = 30;

	// 移動距離の制限
	constexpr float kMoveDistance = 175.0f;

	constexpr int kAddScore = 1000;
}

FlyingEnemy::FlyingEnemy(std::weak_ptr<Camera> camera) :
	m_handleFly(-1),
	m_handleDead(-1),
	m_seHandle(-1),
	m_isDirLeft(false),
	m_blinkFrameCount(0),
	m_isDead(false),
	m_deadAnim(),
	m_moveDir(1),
	m_initPosY(0.0f),
	EnemyBase(Vec3(0.0f, 0.0f), camera)
{
}

FlyingEnemy::~FlyingEnemy()
{
}

void FlyingEnemy::Init(float posX, float posY)
{
	m_handleFly = LoadGraph("data/image/Enemy/But/Flight.png");
	assert(m_handleFly != -1);

	m_handleDead = LoadGraph("data/image/Effect/effect2.png");
	assert(m_handleDead != -1);

	m_seHandle = LoadSoundMem("data/sound/deadSE2.mp3");
	assert(m_seHandle != -1);

	m_idleAnim.Init(m_handleFly, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kWalkAnimNum);
	m_deadAnim.Init(m_handleDead, kDeadAnimSingleFrame, kEffectGraphWidth, kEffectGraphHeight, kEffectExtRate, kRotaRate, kDeadAnimNum);

	m_hp = kDefaultHp;

	m_pos.x = posX;
	m_pos.y = posY;

	m_initPosY = posY;
}

void FlyingEnemy::Update(Player& player, Map& map)
{
	UpdateBlinkFrame();

	if (m_isDead)
	{
		m_deadAnim.Update();
		PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK, true);
		if (m_deadAnim.IsEnd())
		{
			DeleteGraph(m_handleFly);
			DeleteGraph(m_handleDead);
			DeleteGraph(m_seHandle);
		}
	}

	if (!m_isDead)
	{
		m_idleAnim.Update();

		// 移動処理
		m_pos.y += kSpeed * m_moveDir;

		// 一定距離移動したら方向を反転
		if (std::abs(m_pos.y - m_initPosY) >= kMoveDistance)
		{
			m_moveDir *= -1;
		}

		if (m_hp > 0)
		{
			if (GetLeft() < player.GetRight() &&
				GetRight() > player.GetLeft() &&
				GetTop() < player.GetBottom() &&
				GetBottom() > player.GetTop())
			{
				player.OnDamage();
			}
		}
	}

	// 死亡
	if (m_hp <= 0 && !m_isDead)
	{
		m_isDead = true;
		player.GetScoreManager().AddScore(kAddScore);
	}
}

void FlyingEnemy::Draw()
{
	// 点滅処理
	if ((m_blinkFrameCount / 2) % 2)
	{
		return;
	}

	Vec3 camOffset = m_camera.lock()->GetDrawOffset();
	camOffset.x = 0;

#ifdef DISP_COLLISION
	if (m_hp > 0)
	{
		DrawBox(static_cast<int>(GetLeft()), static_cast<int>(GetTop() + camOffset.y),
			static_cast<int>(GetRight()), static_cast<int>(GetBottom() + camOffset.y), 0xff0000, false);
	}
#endif

	if (m_isDead)
	{
		m_deadAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
	else
	{
		m_idleAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
}

void FlyingEnemy::End()
{
	DeleteGraph(m_handleFly);
}

float FlyingEnemy::GetLeft()
{
	return (m_pos.x - 40);
}

float FlyingEnemy::GetRight()
{
	return (m_pos.x + 40);
}

float FlyingEnemy::GetTop()
{
	return (m_pos.y);
}

float FlyingEnemy::GetBottom()
{
	return (m_pos.y + 80);
}

Rect FlyingEnemy::GetRect()
{
	// 矩形当たり判定を作成
	Rect rect;
	rect.top = GetTop();
	rect.bottom = GetBottom();
	rect.right = GetRight();
	rect.left = GetLeft();
	return rect;
}

void FlyingEnemy::OnDamage()
{
	// 既にダメージを受けている(無敵時間は)
	// 再度ダメージを受けることは無い
	if (m_blinkFrameCount > 0) return;

	// 無敵時間(点滅する時間)を設定する
	m_blinkFrameCount = kDamageBlinkFrame;

	// ダメージを受ける
	m_hp--;
}

void FlyingEnemy::UpdateBlinkFrame()
{
	m_blinkFrameCount--;
	if (m_blinkFrameCount < 0)
	{
		m_blinkFrameCount = 0;
	}
}