#include "ClearScene.h"
#include "TitleScene.h"
#include "SceneController.h"

#include "game.h"
#include "Input.h"

#include "DxLib.h"
#include <memory>
#include <cassert>

namespace
{
	// ��ʒ���
	constexpr int kGameScreenHalfWidth = Game::kScreenWidth / 2;
	constexpr int kGameScreenHalfHeight = Game::kScreenHeight / 2;

	constexpr int kFadeInterval = 60;
}

ClearScene::ClearScene(SceneController& controller, int finalScore) :
	SceneBase(controller),
	m_finalScore(finalScore),
	m_update(&ClearScene::FadeInUpdate),
	m_draw(&ClearScene::FadeDraw),
	m_handle(-1)
{
	m_fadeFrameCount = kFadeInterval;
}

void ClearScene::Update(Input& input)
{
	(this->*m_update)(input);
}

void ClearScene::NormalUpdate(Input& input)
{
	if (input.IsPress(PAD_INPUT_3))
	{
		m_update = &ClearScene::FadeOutUpdate;
		m_draw = &ClearScene::FadeDraw;
		m_fadeFrameCount = 0;
	}
}

void ClearScene::FadeInUpdate(Input&)
{
	if (m_fadeFrameCount-- <= 0)
	{
		m_update = &ClearScene::NormalUpdate;
		m_draw = &ClearScene::NormalDraw;
	}
}

void ClearScene::FadeOutUpdate(Input&)
{
	if (m_fadeFrameCount++ >= kFadeInterval)
	{
		// ����ChangeScene���Ăяo���ꂽ�����TitleScene�I�u�W�F�N�g�͏��ł��Ă���
		// ���̌�ɉ��������ƁA���񂾃������ɃA�N�Z�X���ăN���b�V������
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));

		// ����������ł���̂ł����]�v�ȏ����������Ă���Ƃ܂����̂�return;
		return;
	}
}

void ClearScene::Draw()
{
	(this->*m_draw)();
}

void ClearScene::NormalDraw()
{
	if (m_finalScore > 55000)
	{
		DrawFormatString(500, 400, 0xffffff, "S�����N�@Score�F%d", m_finalScore);
	}
	else if (m_finalScore > 45000)
	{
		DrawFormatString(500, 400, 0xffffff, "A�����N�@Score�F%d", m_finalScore);
	}
	else if (m_finalScore > 35000)
	{
		DrawFormatString(500, 400, 0xffffff, "B�����N�@Score�F%d", m_finalScore);
	}
	else if (m_finalScore > 30000)
	{
		DrawFormatString(500, 400, 0xffffff, "C�����N�@Score�F%d", m_finalScore);
	}
	else
	{
		DrawFormatString(500, 400, 0xffffff, "D�����N�@Score�F%d", m_finalScore);
	}
	DrawGraph(0, 0, m_handle, true);
}

void ClearScene::FadeDraw()
{
	DrawGraph(0, 0, m_handle, true);

	float rate = static_cast<float>(m_fadeFrameCount) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}