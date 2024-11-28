#pragma once
#include <fstream>
#include <iostream>
#include <vector>

namespace
{
	constexpr int kMapChipSize = 32;
	constexpr int kMapWidth = 30;
	constexpr int kMapHeight = 100;
	constexpr int kMapGraphSizeW = 1472;
	constexpr int kMapGraphSizeH = 896;
}

class Map
{
public:
	Map();
	~Map();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	int m_handle;

	std::vector<unsigned __int16> m_data;

	int m_fileSize;
};