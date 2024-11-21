#pragma once
#include "EnemyBase.h"
#include "Animation.h"
#include "Bullet.h"

class Bullet;

class Boss : public EnemyBase
{
public:
	Boss();
	~Boss();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

private:
	Bullet m_bullet;
};