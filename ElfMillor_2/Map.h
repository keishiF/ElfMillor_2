#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "game.h"
#include "PlatinumLoader.h"
#include "DxLib.h"
#include "Vec3.h"
#include "Rect.h"
#include "MapChip.h"

class Camera;

namespace MapConsts
{
	constexpr int kMapChipSize = 32;
	constexpr int kMapWidth = 30;
	constexpr int kMapHeight = 150;
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
		782,783, // 鎖の天井
		12,13,14,15,268,269
	};

	constexpr int kWhiteList2[] =
	{
		5649,5650,5651,5652, // 下からはすり抜けて上からは乗れる床
	};

	constexpr int kMapOffsetX = 160;
	constexpr int kMapOffsetY = 2477;
}

class Map
{
public:
	void InitMap();
	// マップの描画
	void DrawMap(Camera& camera);

	MapChip mapChips[MapConsts::kMapHeight][MapConsts::kMapWidth];
	int m_graphHandle;
	PlatinumLoader loader;
	// マップの当たり判定
	bool IsCol(Rect rect, Rect& chipRect, Camera& camera);
	bool IsCol2(Rect rect, Rect& chipRect, Camera& camera);
};