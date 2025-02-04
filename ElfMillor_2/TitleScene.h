#pragma once
#include "SceneBase.h"
class TitleScene :
    public SceneBase
{
public:
	TitleScene(SceneController& controller);

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	int m_fadeFrameCount;

	int m_handle;
	int m_backMovieHandle;

	using UpdateFunc_t = void(TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t m_draw;

	// �ʏ�X�V����
	void NormalUpdate(Input& input);
	// �t�F�[�h�C�����̍X�V����
	void FadeInUpdate(Input&);
	// �t�F�[�h�A�E�g�X�V����
	void FadeOutUpdate(Input&);

	// �ʏ�`��
	void NormalDraw();
	// �t�F�[�h���̕`��
	void FadeDraw();
};