#pragma once
#include "SceneBase.h"
class ClearScene :
    public SceneBase
{
public:
	ClearScene(SceneController& controller, int finalScore);

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	int m_fadeFrameCount;

	int m_handle;

	int m_fontHandle;

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

