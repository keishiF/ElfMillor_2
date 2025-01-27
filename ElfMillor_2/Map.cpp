#include "Map.h" 
#include "Camera.h"

#include "DxLib.h"
#include <cassert>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

void Map::InitMap()
{
	// Platinumデータ読み込み
	loader.Load("img/Map/Map.fmf");

	int mapW, mapH;
	loader.GetMapSize(mapW, mapH);

	// マップ画像読み込み
	m_graphHandle = LoadGraph("img/Map/Map.png");
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
			int inGraphXIdx = (mapChip.chipNo % 256); // 画像内の横インデックス
			int inGraphYIdx = (mapChip.chipNo / 256); // 画像内の縦インデックス
			mapChip.posInGraphX = inGraphXIdx * MapConsts::kMapChipSize;
			mapChip.posInGraphY = inGraphYIdx * MapConsts::kMapChipSize;
		}
	}
}

void Map::DrawMap(std::weak_ptr<Camera> camera)
{
	// マップの描画
	for (int hIndex = 0; hIndex < MapConsts::kMapHeight; hIndex++)
	{
		for (int wIndex = 0; wIndex < MapConsts::kMapWidth; wIndex++)
		{
			const MapChip& mapChip = mapChips[hIndex][wIndex];
			if (mapChip.chipNo > 0)
			{
				if (mapChips[hIndex][wIndex].chipNo == 2) continue;
				
				// カメラの位置に応じて描画位置を補正
				auto leftTopX = static_cast<int>(mapChip.m_pos.x) + MapConsts::kMapOffsetX;
				auto leftTopY = static_cast<int>(mapChip.m_pos.y);
				DrawRectGraph(leftTopX, leftTopY + camera.lock()->GetDrawOffset().y,
					mapChip.posInGraphX, mapChip.posInGraphY,
					MapConsts::kMapChipSize, MapConsts::kMapChipSize,
					m_graphHandle, true);

#ifdef DISP_COLLISION
				float chipBottom = mapChip.m_pos.y + MapConsts::kMapChipSize;
				float chipTop = mapChip.m_pos.y;
				float chipRight = mapChip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = mapChip.m_pos.x + MapConsts::kMapOffsetX;
				/*DrawBox(static_cast<int>(chipLeft), static_cast<int>(chipTop + camera.lock()->GetDrawOffset().y),
					static_cast<int>(chipRight), static_cast<int>(chipBottom + camera.lock()->GetDrawOffset().y), 0xff0000, false);

				DrawCircle(static_cast<int>((chipLeft + chipRight) * 0.5f),
					static_cast<int>((chipTop + chipBottom) * 0.5f) + camera.lock()->GetDrawOffset().y,
					10, 0xff00ff, false);*/
#endif

				/*if (mapChip.chipNo == 1308)
				{
					assert(false);
				}*/
			}
		}
	}
}

bool Map::IsCol(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera)
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
				Vec3 camOffset = camera.lock()->GetDrawOffset();

				// 当たり判定したいやつの上下左右を取る
				MapChip chip = mapChips[y][x];
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize;
				float chipTop = chip.m_pos.y;
				float chipRight = chip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = chip.m_pos.x + MapConsts::kMapOffsetX;

				// 絶対に当たっていない場合continue
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top) continue;
				if (chipRight < rect.left) continue;
				if (chipLeft > rect.right) continue;

				// 当たったマップチップの矩形を設定する
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

bool Map::IsCol2(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera)
{
	// マップの当たり判定
	for (int y = 0; y < MapConsts::kMapHeight; y++)
	{
		for (int x = 0; x < MapConsts::kMapWidth; x++)
		{
			// 当たり判定を取るものを限定する
			// WhiteListに天井や床、壁など...
			for (int i = 0; i < _countof(MapConsts::kWhiteList2); i++)
			{
				if (mapChips[y][x].chipNo != MapConsts::kWhiteList2[i]) continue;

				// カメラに応じて補正
				Vec3 camOffset = camera.lock()->GetDrawOffset();

				// 当たり判定したいやつの上下左右を取る
				MapChip chip = mapChips[y][x];
				float chipTop = chip.m_pos.y;
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize;
				float chipRight = chip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = chip.m_pos.x + MapConsts::kMapOffsetX;

				// 絶対に当たっていない場合continue
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top)    continue;
				if (chipRight < rect.left)   continue;
				if (chipLeft > rect.right)  continue;

				// 当たったマップチップの矩形を設定する
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

bool Map::IsDamageCol(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera)
{
	// マップの当たり判定
	for (int y = 0; y < MapConsts::kMapHeight; y++)
	{
		for (int x = 0; x < MapConsts::kMapWidth; x++)
		{
			// 当たり判定を取るものを限定する
			// WhiteListに天井や床、壁など...
			for (int i = 0; i < _countof(MapConsts::kDamageList); i++)
			{
				if (mapChips[y][x].chipNo != MapConsts::kDamageList[i]) continue;

				// カメラに応じて補正
				Vec3 camOffset = camera.lock()->GetDrawOffset();

				// 当たり判定したいやつの上下左右を取る
				MapChip chip = mapChips[y][x];
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize;
				float chipTop = chip.m_pos.y;
				float chipRight = chip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = chip.m_pos.x + MapConsts::kMapOffsetX;

				// 絶対に当たっていない場合continue
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top)    continue;
				if (chipRight < rect.left)   continue;
				if (chipLeft > rect.right)  continue;

				// 当たったマップチップの矩形を設定する
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
