#pragma once
#include "EnemyBase.h"
#include "Animation.h"
#include "Vec3.h"

class Camera;

class Boss : public EnemyBase
{
public:
	Boss(Camera& camera);
	~Boss();

	virtual void Init(float posX, float posY) override;
	virtual void Update() override;
	virtual void Draw() override;

	void End();
	void Shot();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

private:
	int m_handleIdle;

	int m_handleShot;
};