#pragma once
#include "EnemyBase.h"
#include "Animation.h"

class Player;

class Enemy1 :public EnemyBase
{
public:
	Enemy1();
	~Enemy1();

	virtual void Init() override;
	virtual void Update() override;
	void Update(Player& player);
	virtual void Draw() override;
	void End();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

private:
	int m_handleIdle;
};