#include "ResultScene.h"
#include "DxLib.h"
#include "Input.h"
#include "SceneController.h"
#include "TitleScene.h"

#include <memory>
#include <cassert>

namespace
{
	constexpr int kFadeInterval = 60;
}

void ResultScene::FadeInUpdate(Input& input)
{
	if (m_frame-- <= 0)
	{
		m_update = &ResultScene::NormalUpdate;
		m_draw = &ResultScene::NormalDraw;
	}
}

void ResultScene::FadeOutUpdate(Input& input)
{
	if (m_frame++ >= kFadeInterval)
	{
		// ����ChangeScene���Ăяo���ꂽ�����TitleScene�I�u�W�F�N�g�͏��ł��Ă���
		// ���̌�ɉ��������ƁA���񂾃������ɃA�N�Z�X���ăN���b�V������
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));

		// ����������ł���̂ł����]�v�ȏ����������Ă���Ƃ܂����̂�return;
		return;
	}
}

void ResultScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_1))
	{
		m_update = &ResultScene::FadeOutUpdate;
		m_draw = &ResultScene::FadeDraw;
		m_frame = 0;
	}
}

void ResultScene::FadeDraw()
{
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * rate);
	DrawBox(0, 0, 1280, 720, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ResultScene::NormalDraw()
{
	DrawString(10, 10, "ResultScene", 0xffffff);
}

ResultScene::ResultScene(SceneController& controller):
	SceneBase(controller),
	m_update(&ResultScene::FadeInUpdate),
	m_draw(&ResultScene::FadeDraw)
{
	m_backHandle = LoadGraph("img/BackGround/zoro.png");
	assert(m_backHandle != -1);
	m_frame = kFadeInterval;
}

void ResultScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void ResultScene::Draw()
{
	(this->*m_draw)();
}