#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Animation.h"
#include "GameObject.h"

#include <vector>
#include <memory>

class Input;
class Boss;
class GroundEnemy;
class FlyingEnemy;
class Camera;
class Map;

class Shot
{
public:
	Shot();
	~Shot();

	// 初期化、更新、描画
	void Init();
	void Update(std::vector<std::shared_ptr<GroundEnemy>> groundEnemy, 
		std::vector<std::shared_ptr<FlyingEnemy>> flyingEnemy, std::weak_ptr<Camera> camera, Map& map);
	void Draw(std::weak_ptr<Camera> camera);

	// 矩形情報
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();
	Rect GetRect();

	// 弾が出ているかどうかのフラグ
	bool m_isShot;

	// 弾の位置座標
	Vec3 m_pos;
	Vec3 m_velocity;

	// 左向きかどうか
	bool m_isDirLeft;

	// 上入力されているかのフラグ
	bool m_isUp;

private:

	// 弾のグラフィックハンドル
	int m_shotHandle;
};