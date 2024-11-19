#pragma once
#include "Vec3.h"

class Animation
{
public:
	void Init(int drawHandle, int animAllFrame, int animSingleFrame, int graphWidth, int graphHeight, float expRate, int animNum);
	void Update();
	void Play(Vec3 pos, bool isDirLeft);

private:
	int m_animAllFrame;
	int m_animSingleFrame;
	int m_graphWidth;
	int m_graphHeight;
	float m_expRate;
	int m_animNum;
	int m_totalFrame;

	bool m_isDirLeft;

	int m_drawHandle;
};