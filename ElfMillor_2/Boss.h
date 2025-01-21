#pragma once
#include "EnemyBase.h"
#include "Vec3.h"
#include "Animation.h"

class Camera;

class Boss : public EnemyBase
{
public:
	Boss(std::weak_ptr<Camera> camera);
	~Boss();

	void Init(float posX, float posY) override;
	void Update(Map& map) override;
	void Draw() override;

	void End();
	void Shot();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

private:
	int m_handleIdle;

	int m_handleShot;

	bool m_isDirLeft;
};