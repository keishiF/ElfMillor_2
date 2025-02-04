#pragma once
#include "SceneBase.h"
class ClearScene :
    public SceneBase
{
public:
	ClearScene(SceneController& controller, int finalScore);
	~ClearScene();

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	int m_fadeFrameCount;
	int m_blinkFrameCount;

	int m_bgHandle;
	int m_fontHandle;
	// SEハンドル
	int m_seHandle;

	// BGMハンドル
	int m_bgmHandle;

	int m_finalScore;

	using UpdateFunc_t = void(ClearScene::*)(Input&);
	using DrawFunc_t = void(ClearScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t m_draw;

	// 通常更新処理
	void NormalUpdate(Input& input);
	// フェードイン時の更新処理
	void FadeInUpdate(Input&);
	// フェードアウト更新処理
	void FadeOutUpdate(Input&);

	// 通常描画
	void NormalDraw();
	// フェード時の描画
	void FadeDraw();
};

