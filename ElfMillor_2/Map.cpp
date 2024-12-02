#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

//namespace
//{
//	// マップチップ1つのサイズ
//	constexpr int kMapChipWidth = 32;
//	constexpr int kMapChipHeight = 32;
//
//	// パーツの選択範囲
//	constexpr int kMapPartsNumX= 46;
//	constexpr int kMapPartsNumY = 28;
//
//	// マップチップの要素数(大きさ)
//	constexpr int kMapChipIndexX = 30;
//	constexpr int kMapChipIndexY = 100;
//
//	const char* const kFileName = "img/BackGround/Map.fmf";
//}
//
//Map::Map():
//	m_handle(-1)
//{
//}
//
//Map::~Map()
//{
//}
//
//void Map::Init()
//{
//	// グラフィックの読み込み
//	m_handle = LoadGraph("img/BackGround/Map.jpg");
//	assert(m_handle != -1);
//
//	// fmfファイルの読み込み
//	std::ifstream ifs(kFileName);
//	// 読み込めていないときのエラー表示
//	if (!ifs)
//	{
//		std::cerr << "Error:file not opened." << std::endl;
//	}
//
//	// サイズを調べる
//	ifs.seekg(0, std::ios::end);
//	int size = ifs.tellg();
//	ifs.seekg(0);
//
//	//なぜか最初の20バイトが無駄なデータになっているので
//	//20バイト分の無駄なデータを読み飛ばす
//	for (int i = 0; i < 20; i++)
//	{
//		char tmp;
//		ifs.read(&tmp, sizeof(char));
//	}
//
//	// データを読み込む
//	std::vector<char>data;
//
//	m_data.clear();
//	m_data.resize(size / 2); // 16 bit is 2 bytes.
//
//	ifs.read((char*)m_data.data(), size);
//
//	/*while (true)
//	{
//		ifs.read(&tmp, sizeof(int));
//
//		m_data.push_back(tmp);
//
//		size--;
//
//		if (size <= 0)
//		{
//			break;
//		}
//	}*/
//
//	// データを表示
//	for (int i = 0; i < data.size(); i++)
//	{
//		std::cout << "配列番号" << i << " : " << static_cast<int>(data[i]) << std::endl;
//	}
//}
//
//void Map::End()
//{
//	DeleteGraph(m_handle);
//}
//
//void Map::Update()
//{
//}
//
//void Map::Draw()
//{
//	for (int y = 0; y < kMapChipIndexY; y++)
//	{
//		for (int x = 0; x < kMapChipIndexX; x++)
//		{
//			int chipNo = m_data[(y * kMapChipIndexX) + x];
//
//			if (chipNo < 0)
//			{
//				// continueは繰り返し文(for,while)の中で使用する命令
//				// continueが呼ばれたら以降の繰り返し処理は行わず次のループに移行する
//				continue;
//			}
//
//			int indexX = chipNo % kMapPartsNumX;
//			int indexY = chipNo / kMapPartsNumY;
//
//			int cutX = indexX * kMapChipWidth;
//			int cutY = indexY * kMapChipHeight;
//
//			int xNum = kMapChipHeight * kMapChipIndexY - Game::kScreenHeight;
//
//			DrawRectGraph
//			(x * kMapChipWidth,
//				y * kMapChipHeight - xNum,
//				cutX, cutY,
//				kMapChipWidth, kMapChipHeight,
//				m_handle, true);
//		}
//	}
//}

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

	/*for (int wIndex = 0; wIndex < kMapWidth; wIndex++)
	{
		DrawFormatString(0 * wIndex * 30, 0xffffff, "x:%f y:%f", map.mapChips[wIndex][0].m_pos.x, map.mapChips[wIndex][0].m_pos.x);
	}*/
}