#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "PlatinumLoader.h"
#include "DxLib.h"
#include "Vec3.h"

namespace
{
	constexpr int kMapChipSize = 32;
	constexpr int kMapWidth = 30;
	constexpr int kMapHeight = 24;
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
	//constexpr int kBlackList[] = 
	//{
	//	528,529,784,785,1040,1041,1296,1297,// ��
	//	1038,1039,1294,1295,1550,1551,4110,4111,4366,4367,4622,4623,4878,4879,// ��
	//	1805,1806,1807,1808,
	//}

	constexpr int kMapOffsetX = 160;
	constexpr int kMapOffsetY = 2470;
}

/// <summary>
/// �}�b�v�`�b�v�\����
/// </summary>
struct MapChip
{
	int chipNo;
	float w, h;			// ���A����
	VECTOR m_pos;			// ���W
	int posInGraphX;	// �摜����X���W
	int posInGraphY;	// �摜����Y���W
};

/// <summary>
/// �}�b�v�\����
/// </summary>
struct Map
{
	MapChip mapChips[kMapHeight][kMapWidth];
	int m_graphHandle;
	PlatinumLoader loader;
};

// �v���g�^�C�v�錾
void InitMap(Map& map);
void DrawMap(const Map& map);