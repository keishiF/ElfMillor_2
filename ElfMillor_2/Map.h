#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "PlatinumLoader.h"
#include "DxLib.h"
#include "Vec3.h"

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

//namespace
//{
//	constexpr int kMapChipSize = 32;
//	constexpr int kMapWidth = 30;
//	constexpr int kMapHeight = 100;
//	constexpr int kMapGraphSize = 1472;
//}
//
///// <summary>
///// �}�b�v�`�b�v�\����
///// </summary>
//struct MapChip
//{
//	int chipNo;
//	float w, h;			// ���A����
//	VECTOR m_pos;			// ���W
//	int posInGraphX;	// �摜����X���W
//	int posInGraphY;	// �摜����Y���W
//};
//
///// <summary>
///// �}�b�v�\����
///// </summary>
//struct Map
//{
//	MapChip mapChips[kMapWidth][kMapHeight];
//	int m_graphHandle;
//	PlatinumLoader loader;
//};
//
//// �v���g�^�C�v�錾
//void InitMap(Map& map);
//void DrawMap(const Map& map);