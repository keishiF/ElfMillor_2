#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include "Player.h"
#include "Camera.h"
#include <cassert>

void Map::InitMap()
{
	// Platinum�f�[�^�ǂݍ���
	//map.loader.Load("img/BackGround/Map.fmf");
	loader.Load("img/BackGround/MapTest.fmf");

	int mapW, mapH;
	loader.GetMapSize(mapW, mapH);

	// �}�b�v�摜�ǂݍ���
	m_graphHandle = LoadGraph("img/BackGround/Map.png");
	// �}�b�v�f�[�^�ǂݍ���
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

			// �|�W�V�����̊�������
			mapChip.m_pos = Vec3(wIndex * mapChip.w, hIndex * mapChip.h);

			// �摜���ł̕`��J�n���W�i����j
			// MapGraphSize=512 MapChipSize=32 512*512�̉摜
			// 512 / 32 = 16�Ȃ̂ŁA�P�s16�~16��̉摜������
			// ���ォ�珇�Ԃ�1���ԍ������Ă���
			// 0   1  2  .... 15
			// 16 17 18  .... 31
			// .................
			// 240 ..........255
			// �Ɣԍ����t���̂ŁA���̔ԍ��~MapChipSize(32)���摜���̍��W
			int chipSideNum = MapConsts::kMapGraphSize / MapConsts::kMapChipSize;  // ��Ђ̃}�b�v�`�b�v��
			int inGraphXIdx = (mapChip.chipNo % 256); // �摜���̉��C���f�b�N�X
			int inGraphYIdx = (mapChip.chipNo / 256); // �摜���̏c�C���f�b�N�X
			mapChip.posInGraphX = inGraphXIdx * MapConsts::kMapChipSize;
			mapChip.posInGraphY = inGraphYIdx * MapConsts::kMapChipSize;
		}
	}
}

void Map::DrawMap(Camera& camera)
{
	// �}�b�v�̕`��
	for (int hIndex = 0; hIndex < MapConsts::kMapHeight; hIndex++)
	{
		for (int wIndex = 0; wIndex < MapConsts::kMapWidth; wIndex++)
		{
			const MapChip& mapChip = mapChips[hIndex][wIndex];
			if (mapChip.chipNo > 0)
			{
				// �J�����̈ʒu�ɉ����ĕ`��ʒu��␳
				auto leftTopX = static_cast<int>(mapChip.m_pos.x) + MapConsts::kMapOffsetX;
				auto leftTopY = static_cast<int>(mapChip.m_pos.y) - MapConsts::kMapOffsetY;
				DrawRectGraph(leftTopX + camera.GetDrawOffset().x, leftTopY + camera.GetDrawOffset().y,
					mapChip.posInGraphX, mapChip.posInGraphY,
					MapConsts::kMapChipSize, MapConsts::kMapChipSize,
					m_graphHandle, true);

				if (mapChip.chipNo == 1308)
				{
					assert(false);
				}
			}
		}
	}
}

bool Map::IsCol(Rect rect, Rect& chipRect, Camera& camera)
{	
	// �}�b�v�̓����蔻��
	for (int y = 0; y < MapConsts::kMapHeight; y++)
	{
		for (int x = 0; x < MapConsts::kMapWidth; x++)
		{
			// �����蔻��������̂����肷��
			// WhiteList�ɓV��⏰�A�ǂȂ�...
			for (int i = 0; i < _countof(MapConsts::kWhiteList); i++)
			{
				if (mapChips[y][x].chipNo != MapConsts::kWhiteList[i]) continue;

				// �J�����ɉ����ĕ␳
				Vec3 camOffset = camera.GetDrawOffset();

				// �����蔻�肵������̏㉺���E�����
				MapChip chip = mapChips[y][x];
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize * 0.5 - MapConsts::kMapOffsetY + 16;
				float chipTop = chip.m_pos.y - MapConsts::kMapChipSize * 0.5 - MapConsts::kMapOffsetY + 16;
				float chipRight = chip.m_pos.x + MapConsts::kMapChipSize * 0.5 + MapConsts::kMapOffsetX + 16;
				float chipLeft = chip.m_pos.x - MapConsts::kMapChipSize * 0.5 + MapConsts::kMapOffsetX + 16;
				DrawBox(chipLeft + camOffset.x, chipTop + camOffset.y, chipRight + camOffset.x, chipBottom + camOffset.y, 0xff0000, false);

				// �������Ă��Ȃ��̂ň�x���[�v����o��
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top) continue;
				if (chipRight < rect.left) continue;
				if (chipLeft > rect.right) continue;

				chipRect.top = chipTop;
				chipRect.bottom = chipBottom;
				chipRect.right = chipRight;
				chipRect.left = chipLeft;

				// �������Ă���
				return true;
			}
		}
	}	

	return false;
}