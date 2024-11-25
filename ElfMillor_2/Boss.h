#pragma once
#include "EnemyBase.h"
#include "Animation.h"

class Boss : public EnemyBase
{
public:
	Boss();
	~Boss();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;

	void End();
	void Shot();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

private:
	int m_handleShot;
};