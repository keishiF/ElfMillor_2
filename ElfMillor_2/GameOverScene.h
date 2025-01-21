#pragma once
#include "SceneBase.h"

class GameOverScene :
    public SceneBase
{
public:
	GameOverScene(SceneController& controller);

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	int m_frame = 60;

	int m_handle;

	using UpdateFunc_t = void (GameOverScene::*)(Input& input);
	using DrawFunc_t   = void (GameOverScene::*)();

	UpdateFunc_t m_update = nullptr;
	DrawFunc_t   m_draw = nullptr;

	void NormalUpdate(Input& input);
	void FadeInUpdate(Input& input);
	void FadeOutUpdate(Input& input);

	void NormalDraw();
	void FadeDraw();
};