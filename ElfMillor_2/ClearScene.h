#pragma once
#include "SceneBase.h"
class ClearScene :
    public SceneBase
{
public:
	ClearScene(SceneController& controller, float finalScore);
	~ClearScene();

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	// フェード用のフレーム
	int m_fadeFrameCount;
	// 文字を点滅させるためのフレーム
	int m_blinkFrameCount;

	// クリア画面のハンドル
	int m_bgHandle;
	// 背景のハンドル
	int m_clearBgHandle;
	// フォントハンドル
	int m_fontHandle;
	// SEハンドル
	int m_seHandle;
	// BGMハンドル
	int m_bgmHandle;

	// ゲームシーンからスコアを受け取り表示するための変数
	float m_finalScore;

	using UpdateFunc_t = void(ClearScene::*)(Input&);
	using DrawFunc_t = void(ClearScene::*)();

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

