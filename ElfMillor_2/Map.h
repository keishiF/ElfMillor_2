#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "PlatinumLoader.h"
#include "DxLib.h"
#include "Vec3.h"
#include "Player.h"

namespace
{
	constexpr int kMapChipSize = 32;
	constexpr int kMapWidth = 30;
	constexpr int kMapHeight = 100;
	constexpr int kMapGraphSize = 1472;
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
	MapChip mapChips[kMapWidth][kMapHeight];
	int m_graphHandle;
	PlatinumLoader loader;
};

// プロトタイプ宣言
void InitMap(Map& map);
void DrawMap(const Map& map);
void Update(Map& map, Player& player);