#include "Effect.h"

#include "DxLib.h"
#include <cassert>

namespace
{
	// �G�t�F�N�g�̃O���t�B�b�N�T�C�Y
	constexpr int kGraphWidth = 64;
	constexpr int kGraphHeight = 64;

	// �A�j���[�V����1�R�}�̃t���[����
	constexpr int kAnimSingleFrame = 4;

	// �G�t�F�N�g�̃A�j���[�V�����̃R�}��
	constexpr int kEffectAnimNum = 14;

	// �O���t�B�b�N�̊g�嗦
	constexpr float kExtRate = 1.5f;

	// �O���t�B�b�N�̉�]��
	constexpr float kRotaRate = 0.0f;
}

Effect::Effect():
	m_pos(0.0f, 0.0f),
	m_isEffect(false),
	m_isDirLeft(false),
	m_effectHandle(-1),
	m_effectAnim()
{
}

Effect::~Effect()
{
	DeleteGraph(m_effectHandle);
}

void Effect::Init()
{
	m_effectHandle = LoadGraph("data/image/Effect/effect.png");
	assert(m_effectHandle != -1);

	m_effectAnim.Init(m_effectHandle, kAnimSingleFrame, kGraphWidth, kGraphHeight, kExtRate, kRotaRate, kEffectAnimNum);
}

void Effect::Update()
{
	m_effectAnim.Update();
}

void Effect::Draw(Vec3 camOffset)
{
	if (m_isEffect)
	{
		m_effectAnim.Play(m_pos + camOffset, m_isDirLeft);
	}
}