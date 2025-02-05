#include "TitleScene.h"
#include "GameScene.h"
#include "SceneController.h"

#include "game.h"
#include "Input.h"

#include "DxLib.h"
#include <memory>
#include <cassert>

namespace
{
	// ��ʒ���
	constexpr int kGameScreenHalfWidth  = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;
	constexpr int kFadeInterval = 60;

	// �{�^���̍��W
	constexpr int kTitleStringPosX = 330;
	constexpr int kTitleStringPosY = 400;
}

TitleScene::TitleScene(SceneController& controller) :
	SceneBase(controller),
	m_blinkFrameCount(0),
	m_update(&TitleScene::FadeInUpdate),
	m_draw(&TitleScene::FadeDraw),
	m_handle(-1),
	m_backMovieHandle(-1)
{
	m_fadeFrameCount = kFadeInterval;

	m_handle = LoadGraph("data/image/BackGround/title3.png");
	assert(m_handle != -1);

	m_fontHandle = CreateFontToHandle("Algerian", 48, -1, DX_FONTTYPE_ANTIALIASING_8X8);
	assert(m_fontHandle != -1);

	// SE�̓ǂݍ���
	m_seHandle = LoadSoundMem("data/sound/SE/TitleButtonSE.mp3");
	assert(m_seHandle != -1);

	m_backMovieHandle = LoadGraph("data/movie/TitleMovie.mp4");
	ChangeMovieVolumeToGraph(0, m_backMovieHandle);
	PlayMovieToGraph(m_backMovieHandle, DX_PLAYTYPE_LOOP);
}

TitleScene::~TitleScene()
{
	//DeleteSoundMem(m_bgmHandle);
	DeleteGraph(m_backMovieHandle);
}

void TitleScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void TitleScene::NormalUpdate(Input& input)
{
	++m_blinkFrameCount;

	if (input.IsPress(CheckHitKeyAll()))
	{
		PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK, true);
		m_update = &TitleScene::FadeOutUpdate;
		m_draw = &TitleScene::FadeDraw;
		m_fadeFrameCount = 0;
	}
}

void TitleScene::FadeInUpdate(Input&)
{
	if (m_fadeFrameCount-- <= 0)
	{
		m_update = &TitleScene::NormalUpdate;
		m_draw   = &TitleScene::NormalDraw;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (m_fadeFrameCount++ >= kFadeInterval)
	{
		PauseMovieToGraph(m_backMovieHandle);
		// ����ChangeScene���Ăяo���ꂽ�����TitleScene�I�u�W�F�N�g�͏��ł��Ă���
		// ���̌�ɉ��������ƁA���񂾃������ɃA�N�Z�X���ăN���b�V������
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));

		// ����������ł���̂ł����]�v�ȏ����������Ă���Ƃ܂����̂�return;
		return;
	}
}

void TitleScene::Draw()
{
	(this->*m_draw)();
}

void TitleScene::NormalDraw()
{
	DrawExtendGraph(0, 0, 1280, 720, m_backMovieHandle, true);
	DrawGraph(0, 0, m_handle, true);

	// �_�Ō��ʂ̂��߂̏���
	if ((m_blinkFrameCount / 30) % 2 == 0)
	{
		// ������`��
		DrawStringToHandle(kTitleStringPosX, kTitleStringPosY, 
			"PRESS ANY BUTTON\n�@�@�@�@�@�@�@�@�@�@�@START", 0xa0d8ef, m_fontHandle);
	}
}

void TitleScene::FadeDraw()
{
	DrawExtendGraph(0, 0, 1280, 720, m_backMovieHandle, true);
	DrawGraph(0, 0, m_handle, true);

	float rate = static_cast<float>(m_fadeFrameCount) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}