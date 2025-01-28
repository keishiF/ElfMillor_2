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

ClearScene::ClearScene(SceneController& controller) :
	SceneBase(controller),
	m_update(&ClearScene::FadeInUpdate),
	m_draw(&ClearScene::FadeDraw),
	m_handle(-1)
{
	m_frame = kFadeInterval;
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
		m_frame = 0;
	}
}

void ClearScene::FadeInUpdate(Input&)
{
	if (m_frame-- <= 0)
	{
		m_update = &ClearScene::NormalUpdate;
		m_draw = &ClearScene::NormalDraw;
	}
}

void ClearScene::FadeOutUpdate(Input&)
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

void ClearScene::Draw()
{
	(this->*m_draw)();
}

void ClearScene::NormalDraw()
{
	DrawGraph(0, 0, m_handle, true);
}

void ClearScene::FadeDraw()
{
	DrawGraph(0, 0, m_handle, true);

	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	DrawBox(0, 0, 1280, 720, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}