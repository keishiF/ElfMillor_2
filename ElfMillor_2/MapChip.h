#pragma once
#include "Vec3.h"

class MapChip
{
public:
	int chipNo;
	float w, h;			// ���A����
	Vec3 m_pos;			// ���W
	int posInGraphX;	// �摜����X���W
	int posInGraphY;	// �摜����Y���W
};