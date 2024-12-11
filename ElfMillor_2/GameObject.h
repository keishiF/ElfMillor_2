#pragma once

#include "DxLib.h"
#include "Vec3.h"

class Camera;

class GameObject
{
public:
	virtual ~GameObject() {};

	Vec3 m_pos;
	Vec3 dir;
	float w;
	float h;
protected:
	Camera& m_camera;

	GameObject(Vec3 initPos, Camera& camera) :
		m_pos(initPos),
		m_camera(camera)
	{
	}
};