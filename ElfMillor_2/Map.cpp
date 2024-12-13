#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

void InitMap(Map& map)
{
	// Platinumデータ読み込み
	map.loader.Load("img/BackGround/Map.fmf");
	//map.loader.Load("img/BackGround/MapTest.fmf");

	int mapW, mapH;
	map.loader.GetMapSize(mapW, mapH);

	// マップ画像読み込み
	map.m_graphHandle = LoadGraph("img/BackGround/Map.png");
	// マップデータ読み込み
	for (int wIndex = 0; wIndex < MapConsts::kMapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapConsts::kMapHeight; hIndex++)
		{
			MapChip& mapChip = map.mapChips[hIndex][wIndex];

			mapChip.w = MapConsts::kMapChipSize;
			mapChip.h = MapConsts::kMapChipSize;

			// Platinumからのデータを入れる
			//mapChip.chipNo = ;

			if (hIndex == 1)
			{
				int a = 1;
			}
			mapChip.chipNo = map.loader.GetChipSpriteNo(LayerType::BackGround, hIndex, wIndex);

			// ポジションの基準を左上に
			mapChip.m_pos = Vec3(wIndex * mapChip.w, hIndex * mapChip.h);

			// 画像内での描画開始座標（左上）
			// MapGraphSize=512 MapChipSize=32 512*512の画像
			// 512 / 32 = 16なので、１行16個×16列の画像が並ぶ
			// 左上から順番に1ずつ番号をつけていく
			// 0   1  2  .... 15
			// 16 17 18  .... 31
			// .................
			// 240 ..........255
			// と番号が付くので、その番号×MapChipSize(32)が画像内の座標
			int chipSideNum = MapConsts::kMapGraphSize / MapConsts::kMapChipSize;  // 一片のマップチップ個数
			int inGraphXIdx = (mapChip.chipNo % 256); // 画像内の横インデックス
			int inGraphYIdx = (mapChip.chipNo / 256); // 画像内の縦インデックス
			mapChip.posInGraphX = inGraphXIdx * MapConsts::kMapChipSize;
			mapChip.posInGraphY = inGraphYIdx * MapConsts::kMapChipSize;
		}
	}
}

void DrawMap(const Map& map)
{
	for (int hIndex = 0; hIndex < MapConsts::kMapHeight; hIndex++)
	{
		for (int wIndex = 0; wIndex < MapConsts::kMapWidth; wIndex++)
		{
			// マップチップの中で当たり判定したいやつを限定する
			for (int i = 0; i < _countof(MapConsts::kWhiteList); i++)
			{
				if (map.mapChips[hIndex][wIndex].chipNo == MapConsts::kWhiteList[i])
				{
					// 当たり判定したいやつの上下左右を取る
					MapChip chip = map.mapChips[hIndex][wIndex];
					float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize * 0.5 - MapConsts::kMapOffsetY + 16;
					float chipTop = chip.m_pos.y - MapConsts::kMapChipSize * 0.5 - MapConsts::kMapOffsetY + 16;
					float chipRight = chip.m_pos.x + MapConsts::kMapChipSize * 0.5 + MapConsts::kMapOffsetX + 16;
					float chipLeft = chip.m_pos.x - MapConsts::kMapChipSize * 0.5 + MapConsts::kMapOffsetX + 16;
					DrawBox(chipLeft, chipTop, chipRight, chipBottom, 0xffffff, false);
				}
			}

			// １は当たり判定チップを表しているので１のところだけ描画
			const MapChip& mapChip = map.mapChips[hIndex][wIndex];
			if (mapChip.chipNo > 0)
			{
				auto leftTopX = static_cast<int>(mapChip.m_pos.x) + MapConsts::kMapOffsetX;
				auto leftTopY = static_cast<int>(mapChip.m_pos.y) - MapConsts::kMapOffsetY;
				DrawRectGraph(leftTopX, leftTopY,
					mapChip.posInGraphX, mapChip.posInGraphY,
					MapConsts::kMapChipSize, MapConsts::kMapChipSize,
					map.m_graphHandle, true);

				if (mapChip.chipNo == 1308)
				{
					assert(false);
				}

				DrawBox(leftTopX, leftTopY, leftTopX + MapConsts::kMapChipSize, leftTopY + MapConsts::kMapChipSize, 0xff0000, false);
			}
		}
	}
}