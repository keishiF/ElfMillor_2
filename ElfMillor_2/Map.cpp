#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

void InitMap(Map& map)
{
	// Platinum�f�[�^�ǂݍ���
	map.loader.Load("img/BackGround/Map.fmf");
	//map.loader.Load("img/BackGround/MapTest.fmf");

	// �}�b�v�摜�ǂݍ���
	map.m_graphHandle = LoadGraph("img/BackGround/Map.png");
	// �}�b�v�f�[�^�ǂݍ���
	for (int wIndex = 0; wIndex < kMapWidth; wIndex++)
	{
		for (int hIndex = 0; hIndex < kMapHeight; hIndex++)
		{
			MapChip& mapChip = map.mapChips[hIndex][wIndex];
			mapChip.w = kMapChipSize;
			mapChip.h = kMapChipSize;

			// Platinum����̃f�[�^������
			//mapChip.chipNo = ;

			if (hIndex == 1)
			{
				int a = 1;
			}
			mapChip.chipNo = map.loader.GetChipSpriteNo(LayerType::BackGround, hIndex, wIndex);

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
	for (int hIndex = 0; hIndex < kMapHeight; hIndex++)
	{
		for (int wIndex = 0; wIndex < kMapWidth; wIndex++)
		{
			// �P�͓����蔻��`�b�v��\���Ă���̂łP�̂Ƃ��낾���`��
			const MapChip& mapChip = map.mapChips[hIndex][wIndex];
			if (mapChip.chipNo > 0)
			{
				auto leftTopX = static_cast<int>(mapChip.m_pos.x) + kMapOffsetX;
				auto leftTopY = static_cast<int>(mapChip.m_pos.y) - kMapOffsetY;
				DrawRectGraph(leftTopX, leftTopY,
					mapChip.posInGraphX, mapChip.posInGraphY,
					kMapChipSize, kMapChipSize,
					map.m_graphHandle, true);

				if (mapChip.chipNo == 1308)
				{
					assert(false);
				}

				//DrawBox(leftTopX, leftTopY, leftTopX + kMapChipSize, leftTopY + kMapChipSize, 0xff0000, false);
			}
		}
	}
}