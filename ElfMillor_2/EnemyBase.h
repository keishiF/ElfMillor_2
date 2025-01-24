#pragma once
#include "Vec3.h"
#include "Animation.h"
#include "GameObject.h"

#include <memory>

class Player;
class Camera;
class Map;

class EnemyBase : public GameObject
{
public:
	EnemyBase(Vec3 initPos, std::weak_ptr<Camera> camera) :
		m_hp(),
		m_handle(),
		m_idleAnim(),
		m_runAnim(),
		GameObject(initPos, camera)
	{
	}
	virtual ~EnemyBase() {}

	void SetHandle(int handle) { m_handle = handle; }

	virtual void Init(float posX, float posY) { m_pos.x = posX, m_pos.y = posY; };
	virtual void Update(Player& player, Map& map) = 0;
	virtual void Draw() = 0;

	// 表示位置
	Vec3 m_vec;

	// HP
	int m_hp;

protected:
	// グラフィック
	int m_handle;

	// アニメーション
	Animation m_idleAnim;
	Animation m_runAnim;
};