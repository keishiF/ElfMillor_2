#include "ScoreManager.h"
#include "DxLib.h"

ScoreManager::ScoreManager():
	m_score(0)
{
	m_fontHandle = CreateFontToHandle("Algerian", 25, -1, DX_FONTTYPE_ANTIALIASING_8X8);
}

ScoreManager::~ScoreManager()
{

}

void ScoreManager::Draw()
{
	DrawFormatStringToHandle(10, 60, 0xfff3b8, m_fontHandle, "Score:%d", m_score);
}