#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

void Map::InitMap()
{
	// Platinumデータ読み込み
	//map.loader.Load("img/BackGround/Map.fmf");
	loader.Load("img/BackGround/MapTest.fmf");

	int mapW, mapH;
	loader.GetMapSize(mapW, mapH);

	// マップ画像読み込み
	m_graphHandle = LoadGraph("img/BackGround/Map.png");
	// マップデータ読み込み
	for (int wIndex = 0; wIndex < MapConsts::kMapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < MapConsts::kMapHeight; hIndex++)
		{
			MapChip& mapChip = mapChips[hIndex][wIndex];

			mapChip.w = MapConsts::kMapChipSize;
			mapChip.h = MapConsts::kMapChipSize;

			if (hIndex == 1)
			{
				int a = 1;
			}
			mapChip.chipNo = loader.GetChipSpriteNo(LayerType::BackGround, hIndex, wIndex);

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

void Map::DrawMap(Camera& camera)
{
	// マップの描画
	for (int hIndex = 0; hIndex < MapConsts::kMapHeight; hIndex++)
	{
		for (int wIndex = 0; wIndex < MapConsts::kMapWidth; wIndex++)
		{
			const MapChip& mapChip = mapChips[hIndex][wIndex];
			if (mapChip.chipNo > 0)
			{
				// カメラの位置に応じて描画位置を補正
				auto leftTopX = static_cast<int>(mapChip.m_pos.x) + MapConsts::kMapOffsetX;
				auto leftTopY = static_cast<int>(mapChip.m_pos.y) - MapConsts::kMapOffsetY;
				DrawRectGraph(leftTopX + camera.GetDrawOffset().x, leftTopY + camera.GetDrawOffset().y,
					mapChip.posInGraphX, mapChip.posInGraphY,
					MapConsts::kMapChipSize, MapConsts::kMapChipSize,
					m_graphHandle, true);

				if (mapChip.chipNo == 1308)
				{
					assert(false);
				}
			}
		}
	}
}

bool Map::IsCol(Rect rect, Rect& chipRect, Camera& camera)
{	
	// マップの当たり判定
	for (int y = 0; y < MapConsts::kMapHeight; y++)
	{
		for (int x = 0; x < MapConsts::kMapWidth; x++)
		{
			// 当たり判定を取るものを限定する
			// WhiteListに天井や床、壁など...
			for (int i = 0; i < _countof(MapConsts::kWhiteList); i++)
			{
				if (mapChips[y][x].chipNo != MapConsts::kWhiteList[i]) continue;

				// カメラに応じて補正
				Vec3 camOffset = camera.GetDrawOffset();

				// 当たり判定したいやつの上下左右を取る
				MapChip chip = mapChips[y][x];
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize * 0.5 - MapConsts::kMapOffsetY + 16;
				float chipTop = chip.m_pos.y - MapConsts::kMapChipSize * 0.5 - MapConsts::kMapOffsetY + 16;
				float chipRight = chip.m_pos.x + MapConsts::kMapChipSize * 0.5 + MapConsts::kMapOffsetX + 16;
				float chipLeft = chip.m_pos.x - MapConsts::kMapChipSize * 0.5 + MapConsts::kMapOffsetX + 16;
				DrawBox(chipLeft + camOffset.x, chipTop + camOffset.y, chipRight + camOffset.x, chipBottom + camOffset.y, 0xff0000, false);

				// 当たっていないので一度ループから出る
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top) continue;
				if (chipRight < rect.left) continue;
				if (chipLeft > rect.right) continue;

				chipRect.top = chipTop;
				chipRect.bottom = chipBottom;
				chipRect.right = chipRight;
				chipRect.left = chipLeft;

				// 当たっている
				return true;
			}
		}
	}	

	return false;
}