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
	constexpr int kMapHeight = 100;
	constexpr int kMapGraphSize = 1472;
	constexpr int kWhiteList[] =
	{
		1554,1555,1556,1557,// ëê
		1811,1812,1813,1814,// ìy
		523,779,1035,1291,1547,1803,// ç∂ï«
		514,770,1026,1282,1538,1794,// âEï«
		3088,3089,3090,3091,// è∞
		1027,1283,// ï«
		16,17,// è∞
		272,273,// ä¯ÇÃìVà‰
		782,783 // çΩÇÃìVà‰
	};

	constexpr int kMapOffsetX = 160;
	constexpr int kMapOffsetY = 2477;
}

class Map
{
public:
	void InitMap();
	void DrawMap(Camera& camera);

	MapChip mapChips[MapConsts::kMapHeight][MapConsts::kMapWidth];
	int m_graphHandle;
	PlatinumLoader loader;
	bool IsCol(Rect rect, Rect& chipRect, Camera& camera);
};