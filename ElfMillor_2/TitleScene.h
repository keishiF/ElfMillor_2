#pragma once
#include "SceneBase.h"
class TitleScene :
    public SceneBase
{
	int m_frame;
	int m_backHandle;

	using UpdateFunc_t = void(TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t m_draw;

	// フェードイン時の更新処理
	void FadeInUpdate(Input&);
	// フェードアウト更新処理
	void FadeOutUpdate(Input&);
	// 通常更新処理
	void NormalUpdate(Input& input);

	// フェード時の描画
	void FadeDraw();
	// 通常描画
	void NormalDraw();

public:
	TitleScene(SceneController& controller);

	virtual void Update(Input& input)override;
	virtual void Draw()override;
};