#pragma once
#include "SceneBase.h"
class TitleScene :
    public SceneBase
{
public:
	TitleScene(SceneController& controller);
	~TitleScene();

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	// フェード用のフレーム
	int m_fadeFrameCount;

	// 文字を点滅させるためのフレーム
	int m_blinkFrameCount;

	// タイトルのハンドル
	int m_handle;

	// タイトル画面で流す動画のハンドル
	int m_backMovieHandle;

	// フォントハンドル
	int m_fontHandle;

	// SEハンドル
	int m_seHandle;

	using UpdateFunc_t = void(TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t m_draw;

	// 通常更新処理
	void NormalUpdate(Input& input);
	// フェードイン
	void FadeInUpdate(Input&);
	// フェードアウト
	void FadeOutUpdate(Input&);

	// 通常描画
	void NormalDraw();
	// フェード時の描画
	void FadeDraw();
};