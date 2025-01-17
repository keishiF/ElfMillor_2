#pragma once
#include "EnemyBase.h"
#include "Animation.h"
#include "Rect.h"

class Player;
class Camera;
class Map;

class Enemy1 :public EnemyBase
{
public:
	Enemy1(Camera& camera);
	~Enemy1();

	virtual void Init(float posX, float posY) override;
	virtual void Update() override;
	virtual void Draw() override;
	void Update(Map& map);
	void Draw(Camera& camera);
	void End();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect  GetRect();

	void OnDamage();

private:
	int  m_handleRun;

	bool m_isDirLeft;

	// ”í’eŽž‚Ì“_–Å
	int m_blinkFrameCount;
};