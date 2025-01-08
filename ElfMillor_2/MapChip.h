#pragma once
#include "Vec3.h"

class MapChip
{
public:
	int chipNo;
	float w, h;			// 幅、高さ
	Vec3 m_pos;			// 座標
	int posInGraphX;	// 画像内のX座標
	int posInGraphY;	// 画像内のY座標
};