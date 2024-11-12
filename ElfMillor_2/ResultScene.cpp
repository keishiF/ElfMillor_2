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
}

void ResultScene::NormalUpdate(Input& input)
{
}

void ResultScene::FadeDraw()
{
}

void ResultScene::NormalDraw()
{
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
}

void ResultScene::Draw()
{
}