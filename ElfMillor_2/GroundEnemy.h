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
	GroundEnemy(std::weak_ptr<Camera> camera);
	~GroundEnemy();

	void Init(float posX, float posY) override;
	void Update(Map& map) override;
	void Draw() override;
	void End();

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect  GetRect();

	void OnDamage();

private:
	void UpdateBlinkFrame();

	int  m_handleRun;

	bool m_isDirLeft;

	// ”í’eŽž‚Ì“_–Å
	int m_blinkFrameCount;
};