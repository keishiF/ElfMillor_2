#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

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
}

void Update(Map& map, Player& player)
{

}