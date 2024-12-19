#pragma once

#include "Vec3.h"

// ��`�̃f�[�^�Ǘ�
class Rect
{
public:
	float top;
	float bottom;
	float left;
	float right;
public:
	Rect() : //�R���X�g���N�^�ŏ�����
		top(0),
		bottom(0),
		left(0),
		right(0)
	{
	}
	float getWidth() const { return right - left; }
	float getHeight() const { return bottom - top; }

	// �e���W�擾
	Vec3 getCenter();

	// ������(�d�Ȃ�)����
	bool isCol(const Rect& rect);
	// ���W����`���Ɋ܂܂�邩�ǂ�������
};