#pragma once
class ScoreManager
{
public:
	ScoreManager();
	~ScoreManager();

	// スコアの加算と減算
	void AddScore(int score) { m_score += score; }
	void SubScore(int score) { m_score -= score; }
	// スコア取得
	int GetScore() { return m_score; }

	// スコアの表示
	void Draw();

private:
	// スコア
	int m_score;

	// フォントハンドル
	int m_fontHandle;
};