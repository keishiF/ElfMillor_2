#include "Life.h"
#include "DxLib.h"

namespace
{
	// グラフィックサイズ
	constexpr int kGraphWidth = 256;
	constexpr int kGraphHeight = 256;

	constexpr int kBasePosX = 180;
	constexpr int kIntervalX = 40;
	constexpr int kPosY = 30;

	constexpr float kScale = 0.25f;
}

Life::Life():
	m_handle(-1),
	m_index(0)
{
}

Life::~Life()
{
}

void Life::Init()
{
}

void Life::Update()
{
}

void Life::Draw()
{
	int posX = kBasePosX + m_index * kIntervalX;

	DrawRectRotaGraph(posX, kPosY,
		0, 0, kGraphWidth, kGraphHeight,
		kScale, 0.0f, m_handle, true);
}