#pragma once
#include "SceneBase.h"

class ResultScene :
    public SceneBase
{
private:
	int m_frame = 60;
	using UpdateFunc_t = void (ResultScene::*)(Input& input);
	using DrawFunc_t   = void (ResultScene::*)();

	UpdateFunc_t m_update = nullptr;
	DrawFunc_t   m_draw = nullptr;

	void FadeInUpdate(Input& input);
	void FadeOutUpdate(Input& input);
	void NormalUpdate(Input& input);

	void FadeDraw();
	void NormalDraw();

public:
	ResultScene(SceneController& controller);

	virtual void Update(Input& input)override;
	virtual void Draw()override;
};