#pragma once
#include "SceneBase.h"

class GameOverScene :
    public SceneBase
{
public:
	GameOverScene(SceneController& controller);
	~GameOverScene();

	virtual void Update(Input& input)override;
	virtual void Draw()override;

private:
	// �t�F�[�h�p�̃t���[��
	int m_fadeFrameCount = 60;

	// ������_�ł����邽�߂̃t���[��
	int m_blinkFrameCount;

	// �w�i�̃n���h��
	int m_bgHandle;

	// �t�H���g�n���h��
	int m_fontHandle;

	// SE�n���h��
	int m_seHandle;

	// BGM�n���h��
	int m_bgmHandle;

	using UpdateFunc_t = void (GameOverScene::*)(Input& input);
	using DrawFunc_t   = void (GameOverScene::*)();

	UpdateFunc_t m_update = nullptr;
	DrawFunc_t   m_draw = nullptr;

	// �ʏ�̍X�V����
	void NormalUpdate(Input& input);
	// �t�F�[�h�C��
	void FadeInUpdate(Input& input);
	// �t�F�[�h�A�E�g
	void FadeOutUpdate(Input& input);

	// �ʏ�`��
	void NormalDraw();
	// �t�F�[�h���̕`��
	void FadeDraw();
};