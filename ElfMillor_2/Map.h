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
		1554,1555,1556,1557,// 草
		1811,1812,1813,1814,// 土
		523,779,1035,1291,1547,1803,// 左壁
		514,770,1026,1282,1538,1794,// 右壁
		3088,3089,3090,3091,// 床
		1027,1283,// 壁
		16,17,// 床
		272,273,// 旗の天井
		782,783 // 鎖の天井
	};
	//constexpr int kBlackList[] = 
	//{
	//	528,529,784,785,1040,1041,1296,1297,// 旗
	//	1038,1039,1294,1295,1550,1551,4110,4111,4366,4367,4622,4623,4878,4879,// 鎖
	//	1805,1806,1807,1808,
	//}

	constexpr int kMapOffsetX = 160;
	constexpr int kMapOffsetY = 2470;
}

/// <summary>
/// マップチップ構造体
/// </summary>
struct MapChip
{
	int chipNo;
	float w, h;			// 幅、高さ
	VECTOR m_pos;			// 座標
	int posInGraphX;	// 画像内のX座標
	int posInGraphY;	// 画像内のY座標
};

/// <summary>
/// マップ構造体
/// </summary>
struct Map
{
	MapChip mapChips[kMapHeight][kMapWidth];
	int m_graphHandle;
	PlatinumLoader loader;
};

// プロトタイプ宣言
void InitMap(Map& map);
void DrawMap(const Map& map);