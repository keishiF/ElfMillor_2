#pragma once
#include "Vec3.h"

class Animation
{
public:
	void Init(int drawHandle, int animSingleFrame, int graphWidth, int graphHeight, float extRate, float rotaRate, int animNum);
	void Update();
	void Play(Vec3 pos, bool isDirLeft);
	bool IsEnd() const;

private:
	int   m_animFrame;
	int   m_animSingleFrame;
	int   m_graphWidth;
	int   m_graphHeight;
	int   m_animNum;
	int   m_totalFrame;
	float m_extRate;
	float m_rotationRate;

	bool  m_isDirLeft;

	int   m_drawHandle;
};