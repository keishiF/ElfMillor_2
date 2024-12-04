#pragma once
#include "Vec3.h"

class Player;
class Camera
{
public:

	Camera();
	~Camera();

	void Init(Player& player);
	void End();
	void Update(Player& player);

	Vec3 m_pos;
	Vec3 m_offset;
};