#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Animation.h"
#include "GameObject.h"

class Input;
class Boss;
class Enemy1;
class Camera;
class Map;

class Shot
{
public:
	Shot();
	~Shot();

	void Init();
	void Update(Boss& boss, Enemy1& enemy1, Camera& camera, Map& map);
	void Draw(Camera& camera);

	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();

	Rect GetRect();

	// 弾が出ているかどうかのフラグ
	bool m_isShotFlag;

	// 弾の位置座標
	Vec3 m_pos;
	Vec3 m_velocity;

	// 左向きかどうか
	bool m_isDirLeft;

	// 上入力されているかどうか
	bool m_isUp;

private:
	// 弾のグラフィックハンドル
	int m_handle;

	// アニメーション
	Animation m_shotAnim;
};