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

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;
	void Update(Player& player, Map& map);
	void Draw(Camera& camera);
	void End();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect  GetRect();

private:
	int  m_handleRun;

	bool m_isDirLeft;
};