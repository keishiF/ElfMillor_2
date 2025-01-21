#pragma once
#include "EnemyBase.h"
#include "Rect.h"
#include "Animation.h"

class Player;
class Camera;
class Map;

class GroundEnemy :public EnemyBase
{
public:
	GroundEnemy(Camera& camera);
	virtual ~GroundEnemy() {};

	virtual void Init(float posX, float posY) override;
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