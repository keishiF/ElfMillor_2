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

	// �t�F�[�h�C�����̍X�V����
	void FadeInUpdate(Input&);
	// �t�F�[�h�A�E�g�X�V����
	void FadeOutUpdate(Input&);
	// �ʏ�X�V����
	void NormalUpdate(Input& input);

	// �t�F�[�h���̕`��
	void FadeDraw();
	// �ʏ�`��
	void NormalDraw();

public:
	TitleScene(SceneController& controller);

	virtual void Update(Input& input)override;
	virtual void Draw()override;
};