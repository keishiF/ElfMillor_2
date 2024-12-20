#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "game.h"
#include "PlatinumLoader.h"
#include "DxLib.h"
#include "Vec3.h"
#include "Rect.h"

namespace MapConsts
{
	constexpr int kMapChipSize = 32;
	constexpr int kMapWidth = 30;
	constexpr int kMapHeight = 100;
	constexpr int kMapGraphSize = 1472;
	constexpr int kWhiteList[] =
	{
		1554,1555,1556,1557,// ��
		1811,1812,1813,1814,// �y
		523,779,1035,1291,1547,1803,// ����
		514,770,1026,1282,1538,1794,// �E��
		3088,3089,3090,3091,// ��
		1027,1283,// ��
		16,17,// ��
		272,273,// ���̓V��
		782,783 // ���̓V��
	};

	constexpr int kMapOffsetX = 160;
	constexpr int kMapOffsetY = 2477;
}

/// <summary>
/// �}�b�v�`�b�v�\����
/// </summary>
class MapChip
{
public:
	int chipNo;
	float w, h;			// ���A����
	Vec3 m_pos;			// ���W
	int posInGraphX;	// �摜����X���W
	int posInGraphY;	// �摜����Y���W
};

/// <summary>
/// �}�b�v�\����
/// </summary>
class Map
{
public:
	MapChip mapChips[MapConsts::kMapHeight][MapConsts::kMapWidth];
	int m_graphHandle;
	PlatinumLoader loader;
	bool IsCol(Rect rect, Rect& chipRect);
};

// �v���g�^�C�v�錾
void InitMap(Map& map);
void DrawMap(const Map& map);