#pragma once
class ScoreManager
{
public:
	ScoreManager();
	~ScoreManager();

	void AddScore(int score) { m_score += score; }
	void SubScore(int score) { m_score -= score; }
	int GetScore() { return m_score; }

	void Draw();

private:
	int m_score;
};