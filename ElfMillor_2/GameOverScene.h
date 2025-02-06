#pragma once
#include "SceneBase.h"

class GameOverScene :
    public SceneBase
{
public:
	GameOverScene(SceneController& controller);
	~GameOverScene();

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	// フェード用のフレーム
	int m_fadeFrameCount = 60;

	// 文字を点滅させるためのフレーム
	int m_blinkFrameCount;

	// 背景のハンドル
	int m_bgHandle;

	// フォントハンドル
	int m_fontHandle;

	// SEハンドル
	int m_seHandle;

	// BGMハンドル
	int m_bgmHandle;

	using UpdateFunc_t = void (GameOverScene::*)(Input& input);
	using DrawFunc_t   = void (GameOverScene::*)();

	UpdateFunc_t m_update = nullptr;
	DrawFunc_t   m_draw = nullptr;

	// 通常の更新処理
	void NormalUpdate(Input& input);
	// フェードイン
	void FadeInUpdate(Input& input);
	// フェードアウト
	void FadeOutUpdate(Input& input);

	// 通常描画
	void NormalDraw();
	// フェード時の描画
	void FadeDraw();
};