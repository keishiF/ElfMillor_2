#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

//namespace
//{
//	// �}�b�v�`�b�v1�̃T�C�Y
//	constexpr int kMapChipWidth = 32;
//	constexpr int kMapChipHeight = 32;
//
//	// �p�[�c�̑I��͈�
//	constexpr int kMapPartsNumX= 46;
//	constexpr int kMapPartsNumY = 28;
//
//	// �}�b�v�`�b�v�̗v�f��(�傫��)
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
//	// �O���t�B�b�N�̓ǂݍ���
//	m_handle = LoadGraph("img/BackGround/Map.jpg");
//	assert(m_handle != -1);
//
//	// fmf�t�@�C���̓ǂݍ���
//	std::ifstream ifs(kFileName);
//	// �ǂݍ��߂Ă��Ȃ��Ƃ��̃G���[�\��
//	if (!ifs)
//	{
//		std::cerr << "Error:file not opened." << std::endl;
//	}
//
//	// �T�C�Y�𒲂ׂ�
//	ifs.seekg(0, std::ios::end);
//	int size = ifs.tellg();
//	ifs.seekg(0);
//
//	//�Ȃ����ŏ���20�o�C�g�����ʂȃf�[�^�ɂȂ��Ă���̂�
//	//20�o�C�g���̖��ʂȃf�[�^��ǂݔ�΂�
//	for (int i = 0; i < 20; i++)
//	{
//		char tmp;
//		ifs.read(&tmp, sizeof(char));
//	}
//
//	// �f�[�^��ǂݍ���
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
//	// �f�[�^��\��
//	for (int i = 0; i < data.size(); i++)
//	{
//		std::cout << "�z��ԍ�" << i << " : " << static_cast<int>(data[i]) << std::endl;
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
//				// continue�͌J��Ԃ���(for,while)�̒��Ŏg�p���閽��
//				// continue���Ă΂ꂽ��ȍ~�̌J��Ԃ������͍s�킸���̃��[�v�Ɉڍs����
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
	// Platinum�f�[�^�ǂݍ���
	map.loader.Load("img/BackGround/MapTest.fmf");

	// �}�b�v�摜�ǂݍ���
	map.m_graphHandle = LoadGraph("img/BackGround/Map.png");
	// �}�b�v�f�[�^�ǂݍ���
	for (int wIndex = 0; wIndex < kMapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < kMapHeight; hIndex++)
		{
			MapChip& mapChip = map.mapChips[wIndex][hIndex];
			mapChip.w = kMapChipSize;
			mapChip.h = kMapChipSize;

			// Platinum����̃f�[�^������
			//mapChip.chipNo = ;

			if (hIndex == 1)
			{
				int a = 1;
			}
			mapChip.chipNo = map.loader.GetChipSpriteNo(LayerType::BackGround, wIndex, hIndex);

			// �|�W�V�����̊�������
			mapChip.m_pos = VGet(wIndex * mapChip.w, hIndex * mapChip.h, 0);

			// �摜���ł̕`��J�n���W�i����j
			// MapGraphSize=512 MapChipSize=32 512*512�̉摜
			// 512 / 32 = 16�Ȃ̂ŁA�P�s16�~16��̉摜������
			// ���ォ�珇�Ԃ�1���ԍ������Ă���
			// 0   1  2  .... 15
			// 16 17 18  .... 31
			// .................
			// 240 ..........255
			// �Ɣԍ����t���̂ŁA���̔ԍ��~MapChipSize(32)���摜���̍��W
			int chipSideNum = kMapGraphSize / kMapChipSize;  // ��Ђ̃}�b�v�`�b�v��
			int inGraphXIdx = (mapChip.chipNo % 256); // �摜���̉��C���f�b�N�X
			int inGraphYIdx = (mapChip.chipNo / 256); // �摜���̏c�C���f�b�N�X
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
			// �P�͓����蔻��`�b�v��\���Ă���̂łP�̂Ƃ��낾���`��
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