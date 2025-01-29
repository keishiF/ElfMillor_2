#pragma once
#include "Vec3.h"
#include "Animation.h"
#include <memory>

class Effect
{
public:
	Effect();
	~Effect();

	void Init();
	void Update();
	void Draw(Vec3 camOffset);

	Vec3 m_pos;

	bool m_isEffect;

	bool m_isDirLeft;

private:
	int m_effectHandle;

	Animation m_effectAnim;
};