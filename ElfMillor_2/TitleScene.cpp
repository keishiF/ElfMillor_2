#include "TitleScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "GameScene.h"

#include <memory>
#include <cassert>

namespace
{
	constexpr int kFadeInterval = 60;
}

void TitleScene::FadeInUpdate(Input&)
{
	if (m_frame-- <= 0)
	{
		m_update = &TitleScene::NormalUpdate;
		m_draw = &TitleScene::NormalDraw;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (m_frame++ >= kFadeInterval)
	{
		// ����ChangeScene���Ăяo���ꂽ�����TitleScene�I�u�W�F�N�g�͏��ł��Ă���
		// ���̌�ɉ��������ƁA���񂾃������ɃA�N�Z�X���ăN���b�V������
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));

		// ����������ł���̂ł����]�v�ȏ����������Ă���Ƃ܂����̂�return;
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_LEFT))
	{
		m_update = &TitleScene::FadeOutUpdate;
		m_draw = &TitleScene::FadeDraw;
		m_frame = 0;
	}
}

void TitleScene::FadeDraw()
{
}

void TitleScene::NormalDraw()
{
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller),
	m_update(&TitleScene::FadeInUpdate),
	m_draw(&TitleScene::FadeDraw)
{
	m_backHandle = LoadGraph("img/BackGround/chopper.png");
	assert(m_backHandle != -1);
	m_frame = kFadeInterval;
}

void TitleScene::Update(Input& input)
{
}

void TitleScene::Draw()
{
}