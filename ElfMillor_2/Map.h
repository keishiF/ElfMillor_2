#pragma once
#include "MapChip.h"
#include "Vec3.h"
#include "Rect.h"
#include "game.h"
#include "PlatinumLoader.h"

#include <vector>
#include <fstream>
#include <iostream>

class Camera;

namespace MapConsts
{
	constexpr int kMapChipSize = 32;
	constexpr int kMapWidth = 30;
	constexpr int kMapHeight = 250;
	constexpr int kWhiteList[] =
	{
		1554,1555,1556,1557,// ��
		1811,1812,1813,1814,// �y
		523,779,1035,1291,1547,1803,// ����
		514,770,1026,1282,1538,1794,// �E��
		3088,3089,3090,3091,// ��
		1027,1283,// ��
		16,17,// ��
		272,273,// ���̓V��
		782,783, // ���̓V��
		12,13,14,15,268,269
	};

	constexpr int kWhiteList2[] =
	{
		5649,5650,5651,5652, // ������͂��蔲���ďォ��͏��鏰
	};

	constexpr int kDamageList[] =
	{
		4617,4618,4619,4873,4874,4875
	};

	constexpr int kMapOffsetX = 160;
	constexpr int kMapOffsetY = 2477;
}

class Map
{
public:
	void InitMap();
	// �}�b�v�̕`��
	void DrawMap(std::weak_ptr<Camera> camera);

	MapChip mapChips[MapConsts::kMapHeight][MapConsts::kMapWidth];
	int m_graphHandle;
	PlatinumLoader loader;
	// �}�b�v�̓����蔻��
	bool IsCol(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera);

	// ������͂��蔲���A�ォ��͏��鏰�̔���
	bool IsCol2(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera);

	// �_���[�W���󂯂鏰�̔���
	bool IsDamageCol(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera);
};