#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

namespace
{
	constexpr int kMapChipWidth = 32;
	constexpr int kMapChipHeight = 32;

	constexpr int kMapPartsNumX= 46;
	constexpr int kMapPartsNumY = 28;

	constexpr int kMapChipIndexX = 30;
	constexpr int kMapChipIndexY = 100;

	const char* const kFileName = "img/BackGround/Map.fmf";
}

Map::Map():
	m_handle(-1)
{
}

Map::~Map()
{
}

void Map::Init()
{
	m_handle = LoadGraph("img/BackGround/Map.jpg");
	assert(m_handle != -1);

	std::ifstream ifs(kFileName);
	if (!ifs)
	{
		std::cerr << "Error:file not opened." << std::endl;
	}

	ifs.seekg(0, std::ios::end);
	int size = ifs.tellg();
	ifs.seekg(0);

	for (int i = 0; i < 20; i++)
	{
		char tmp;
		ifs.read(&tmp, sizeof(char));
	}

	// データを読み込む
	std::vector<char>data;

	m_data.clear();
	m_data.resize(size / 2); // 16 bit is 2 bytes.

	ifs.read((char*)m_data.data(), size);

	/*while (true)
	{
		ifs.read(&tmp, sizeof(int));

		m_data.push_back(tmp);

		size--;

		if (size <= 0)
		{
			break;
		}
	}*/

	for (int i = 0; i < data.size(); i++)
	{
		std::cout << "配列番号" << i << " : " << static_cast<int>(data[i]) << std::endl;
	}
}

void Map::End()
{
	DeleteGraph(m_handle);
}

void Map::Update()
{
}

void Map::Draw()
{
	for (int y = 0; y < kMapChipIndexY; y++)
	{
		for (int x = 0; x < kMapChipIndexX; x++)
		{
			int chipNo = m_data[(y * kMapChipIndexX) + x];

			if (chipNo < 0)
			{
				// continueは繰り返し文(for,while)の中で使用する命令
				// continueが呼ばれたら以降の繰り返し処理は行わず次のループに移行する
				continue;
			}

			int indexX = chipNo % kMapPartsNumX;
			int indexY = chipNo / kMapPartsNumY;

			int cutX = indexX * kMapChipWidth;
			int cutY = indexY * kMapChipHeight;

			int xNum = kMapChipHeight * kMapChipIndexY - Game::kScreenHeight;

			DrawRectGraph
			(x * kMapChipWidth,
				y * kMapChipHeight - xNum,
				cutX, cutY,
				kMapChipWidth, kMapChipHeight,
				m_handle, true);
		}
	}
}