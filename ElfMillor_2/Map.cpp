#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

void InitMap(Map& map)
{
	// Platinumデータ読み込み
	map.loader.Load("img/BackGround/MapTest.fmf");

	// マップ画像読み込み
	map.m_graphHandle = LoadGraph("img/BackGround/Map.png");
	// マップデータ読み込み
	for (int wIndex = 0; wIndex < kMapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < kMapHeight; hIndex++)
		{
			MapChip& mapChip = map.mapChips[wIndex][hIndex];
			mapChip.w = kMapChipSize;
			mapChip.h = kMapChipSize;

			// Platinumからのデータを入れる
			//mapChip.chipNo = ;

			if (hIndex == 1)
			{
				int a = 1;
			}
			mapChip.chipNo = map.loader.GetChipSpriteNo(LayerType::BackGround, wIndex, hIndex);

			// ポジションの基準を左上に
			mapChip.m_pos = VGet(wIndex * mapChip.w, hIndex * mapChip.h, 0);

			// 画像内での描画開始座標（左上）
			// MapGraphSize=512 MapChipSize=32 512*512の画像
			// 512 / 32 = 16なので、１行16個×16列の画像が並ぶ
			// 左上から順番に1ずつ番号をつけていく
			// 0   1  2  .... 15
			// 16 17 18  .... 31
			// .................
			// 240 ..........255
			// と番号が付くので、その番号×MapChipSize(32)が画像内の座標
			int chipSideNum = kMapGraphSize / kMapChipSize;  // 一片のマップチップ個数
			int inGraphXIdx = (mapChip.chipNo % 256); // 画像内の横インデックス
			int inGraphYIdx = (mapChip.chipNo / 256); // 画像内の縦インデックス
			mapChip.posInGraphX = inGraphXIdx * kMapChipSize;
			mapChip.posInGraphY = inGraphYIdx * kMapChipSize;
		}
	}
}

void DrawMap(const Map& map)
{
	for (int wIndex = 0; wIndex < kMapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < kMapHeight; hIndex++)
		{
			// １は当たり判定チップを表しているので１のところだけ描画
			const MapChip& mapChip = map.mapChips[wIndex][hIndex];
			if (mapChip.chipNo > 0)
			{
				auto leftTopX = static_cast<int>(mapChip.m_pos.x);
				auto leftTopY = static_cast<int>(mapChip.m_pos.y);
				DrawRectGraph(leftTopX + 160, leftTopY - 2450,
					mapChip.posInGraphX, mapChip.posInGraphY,
					kMapChipSize, kMapChipSize,
					map.m_graphHandle, false);

				if (mapChip.chipNo == 1308)
				{
					assert(false);
				}
			}
		}
	}
}

void Update(Map& map, Player& player)
{

}