#pragma once
class ScoreManager
{
public:
	ScoreManager();
	~ScoreManager();

	// �X�R�A�̉��Z�ƌ��Z
	void AddScore(int score) { m_score += score; }
	void SubScore(int score) { m_score -= score; }
	// �X�R�A�擾
	int GetScore() { return m_score; }

	// �X�R�A�̕\��
	void Draw();

private:
	// �X�R�A
	int m_score;

	// �t�H���g�n���h��
	int m_fontHandle;
};