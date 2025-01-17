#pragma once
#include "SceneBase.h"

class GameOverScene :
    public SceneBase
{
private:
	int m_frame = 60;
	using UpdateFunc_t = void (GameOverScene::*)(Input& input);
	using DrawFunc_t   = void (GameOverScene::*)();

	UpdateFunc_t m_update = nullptr;
	DrawFunc_t   m_draw = nullptr;

	int m_handle;

	void FadeInUpdate(Input& input);
	void FadeOutUpdate(Input& input);
	void NormalUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

public:
	GameOverScene(SceneController& controller);

	virtual void Update(Input& input)override;
	virtual void Draw()override;
};