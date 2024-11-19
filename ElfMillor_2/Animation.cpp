#include "Animation.h"
#include "DxLib.h"

void Animation::Init(int drawHandle, int animSingleFrame, int graphWidth, int graphHeight, float expRate, int animNum)
{
	m_animFrame = 0;
	m_drawHandle = drawHandle;
	m_animSingleFrame = animSingleFrame;
	m_graphWidth = graphWidth;
	m_graphHeight = graphHeight;
	m_expRate = expRate;
	m_animNum = animNum;

	m_totalFrame = m_animNum * m_animSingleFrame;
}

void Animation::Update()
{
	++m_animFrame;

	if (m_animFrame >= m_totalFrame)
	{
		m_animFrame = 0;
	}
}

void Animation::Play(Vec3 pos, bool isDirLeft)
{
	int animNo = m_animFrame / m_animSingleFrame;
	DrawRectRotaGraph(pos.x, pos.y,
		animNo * m_graphWidth, 0, m_graphWidth, m_graphHeight,
		m_expRate, 0.0f,
		m_drawHandle, true, isDirLeft);
}