#include "ScoreManager.h"
#include "DxLib.h"

ScoreManager::ScoreManager():
	m_score(0)
{

}

ScoreManager::~ScoreManager()
{

}

void ScoreManager::Draw()
{
	DrawFormatString(10, 60, 0xffffff, "Score�F%d", m_score);
}