#pragma once
#include "Vec3.h"
#include "DxLib.h"

#include <memory>

class Camera;

class GameObject
{
public:
	virtual ~GameObject() {};

	Vec3  m_pos;
	Vec3  dir;

protected:
	std::weak_ptr<Camera> m_camera;

	GameObject(Vec3 initPos, std::weak_ptr<Camera> camera) :
		m_pos(initPos),
		m_camera(camera)
	{
	}
};