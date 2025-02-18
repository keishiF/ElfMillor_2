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
	// �t�F�[�h�p�̃t���[��
	int m_fadeFrameCount;
	// ������_�ł����邽�߂̃t���[��
	int m_blinkFrameCount;

	// �N���A��ʂ̃n���h��
	int m_bgHandle;
	// �w�i�̃n���h��
	int m_clearBgHandle;
	// �t�H���g�n���h��
	int m_fontHandle;
	// SE�n���h��
	int m_seHandle;
	// BGM�n���h��
	int m_bgmHandle;

	// �Q�[���V�[������X�R�A���󂯎��\�����邽�߂̕ϐ�
	float m_finalScore;

	using UpdateFunc_t = void(ClearScene::*)(Input&);
	using DrawFunc_t = void(ClearScene::*)();

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

