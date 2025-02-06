#pragma once

#include "Vec3.h"

// ��`�̃f�[�^�Ǘ�
class Rect
{
public:
	// ��`���
	float top;
	float bottom;
	float left;
	float right;

	Rect() : //�R���X�g���N�^�ŏ�����
		top(0),
		bottom(0),
		left(0),
		right(0)
	{
	}
	// �������
	float GetWidth() const { return right - left; }
	// �c�����
	float GetHeight() const { return bottom - top; }

	// �e���W�擾
	Vec3 GetCenter();

	// ������(�d�Ȃ�)����
	bool IsCol(const Rect& rect);
	// ���W����`���Ɋ܂܂�邩�ǂ�������
};