#include "FlyingEnemy.h"
#include "Player.h"
#include "Camera.h"
#include "Map.h"

#include "DxLib.h"
#include <cassert>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

FlyingEnemy::FlyingEnemy(std::weak_ptr<Camera> camera):
	m_handleRun(-1),
	m_isDirLeft(false),
	m_blinkFrameCount(0),
	EnemyBase(Vec3(0.0f, 0.0f), camera)
{
}

FlyingEnemy::~FlyingEnemy()
{
}

void FlyingEnemy::Init(float posX, float posY)
{
}

void FlyingEnemy::Update(Map& map)
{
}

void FlyingEnemy::Draw()
{
}

void FlyingEnemy::End()
{
}

float FlyingEnemy::GetLeft()
{
	return 0.0f;
}

float FlyingEnemy::GetRight()
{
	return 0.0f;
}

float FlyingEnemy::GetTop()
{
	return 0.0f;
}

float FlyingEnemy::GetBottom()
{
	return 0.0f;
}

Rect FlyingEnemy::GetRect()
{
	return Rect();
}

void FlyingEnemy::OnDamage()
{
}