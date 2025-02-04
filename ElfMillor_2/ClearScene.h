#pragma once
#include "SceneBase.h"
class ClearScene :
    public SceneBase
{
public:
	ClearScene(SceneController& controller, int finalScore);
	~ClearScene();

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	int m_fadeFrameCount;
	int m_blinkFrameCount;

	int m_bgHandle;
	int m_fontHandle;
	// SE�n���h��
	int m_seHandle;

	// BGM�n���h��
	int m_bgmHandle;

	int m_finalScore;

	using UpdateFunc_t = void(ClearScene::*)(Input&);
	using DrawFunc_t = void(ClearScene::*)();

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

