#pragma once
#include <fstream>
#include <iostream>
#include <vector>

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