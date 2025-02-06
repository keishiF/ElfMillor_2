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
	// �t�F�[�h�p�̃t���[��
	int m_fadeFrameCount;

	// ������_�ł����邽�߂̃t���[��
	int m_blinkFrameCount;

	// �^�C�g���̃n���h��
	int m_handle;

	// �^�C�g����ʂŗ�������̃n���h��
	int m_backMovieHandle;

	// �t�H���g�n���h��
	int m_fontHandle;

	// SE�n���h��
	int m_seHandle;

	using UpdateFunc_t = void(TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t m_update;
	DrawFunc_t m_draw;

	// �ʏ�X�V����
	void NormalUpdate(Input& input);
	// �t�F�[�h�C��
	void FadeInUpdate(Input&);
	// �t�F�[�h�A�E�g
	void FadeOutUpdate(Input&);

	// �ʏ�`��
	void NormalDraw();
	// �t�F�[�h���̕`��
	void FadeDraw();
};