#include "Map.h" 
#include "Camera.h"

#include "DxLib.h"
#include <cassert>

#ifdef _DEBUG
#define DISP_COLLISION
#endif

void Map::InitMap()
{
	// Platinum�f�[�^�ǂݍ���
	loader.Load("img/Map/Map.fmf");

	int mapW, mapH;
	loader.GetMapSize(mapW, mapH);

	// �}�b�v�摜�ǂݍ���
	m_graphHandle = LoadGraph("img/Map/Map.png");
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
			int inGraphXIdx = (mapChip.chipNo % 256); // �摜���̉��C���f�b�N�X
			int inGraphYIdx = (mapChip.chipNo / 256); // �摜���̏c�C���f�b�N�X
			mapChip.posInGraphX = inGraphXIdx * MapConsts::kMapChipSize;
			mapChip.posInGraphY = inGraphYIdx * MapConsts::kMapChipSize;
		}
	}
}

void Map::DrawMap(std::weak_ptr<Camera> camera)
{
	// �}�b�v�̕`��
	for (int hIndex = 0; hIndex < MapConsts::kMapHeight; hIndex++)
	{
		for (int wIndex = 0; wIndex < MapConsts::kMapWidth; wIndex++)
		{
			const MapChip& mapChip = mapChips[hIndex][wIndex];
			if (mapChip.chipNo > 0)
			{
				if (mapChips[hIndex][wIndex].chipNo == 2) continue;
				
				// �J�����̈ʒu�ɉ����ĕ`��ʒu��␳
				auto leftTopX = static_cast<int>(mapChip.m_pos.x) + MapConsts::kMapOffsetX;
				auto leftTopY = static_cast<int>(mapChip.m_pos.y);
				DrawRectGraph(leftTopX, leftTopY + camera.lock()->GetDrawOffset().y,
					mapChip.posInGraphX, mapChip.posInGraphY,
					MapConsts::kMapChipSize, MapConsts::kMapChipSize,
					m_graphHandle, true);

#ifdef DISP_COLLISION
				float chipBottom = mapChip.m_pos.y + MapConsts::kMapChipSize;
				float chipTop = mapChip.m_pos.y;
				float chipRight = mapChip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = mapChip.m_pos.x + MapConsts::kMapOffsetX;
				/*DrawBox(static_cast<int>(chipLeft), static_cast<int>(chipTop + camera.lock()->GetDrawOffset().y),
					static_cast<int>(chipRight), static_cast<int>(chipBottom + camera.lock()->GetDrawOffset().y), 0xff0000, false);

				DrawCircle(static_cast<int>((chipLeft + chipRight) * 0.5f),
					static_cast<int>((chipTop + chipBottom) * 0.5f) + camera.lock()->GetDrawOffset().y,
					10, 0xff00ff, false);*/
#endif

				/*if (mapChip.chipNo == 1308)
				{
					assert(false);
				}*/
			}
		}
	}
}

bool Map::IsCol(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera)
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
				Vec3 camOffset = camera.lock()->GetDrawOffset();

				// �����蔻�肵������̏㉺���E�����
				MapChip chip = mapChips[y][x];
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize;
				float chipTop = chip.m_pos.y;
				float chipRight = chip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = chip.m_pos.x + MapConsts::kMapOffsetX;

				// ��΂ɓ������Ă��Ȃ��ꍇcontinue
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top) continue;
				if (chipRight < rect.left) continue;
				if (chipLeft > rect.right) continue;

				// ���������}�b�v�`�b�v�̋�`��ݒ肷��
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

bool Map::IsCol2(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera)
{
	// �}�b�v�̓����蔻��
	for (int y = 0; y < MapConsts::kMapHeight; y++)
	{
		for (int x = 0; x < MapConsts::kMapWidth; x++)
		{
			// �����蔻��������̂����肷��
			// WhiteList�ɓV��⏰�A�ǂȂ�...
			for (int i = 0; i < _countof(MapConsts::kWhiteList2); i++)
			{
				if (mapChips[y][x].chipNo != MapConsts::kWhiteList2[i]) continue;

				// �J�����ɉ����ĕ␳
				Vec3 camOffset = camera.lock()->GetDrawOffset();

				// �����蔻�肵������̏㉺���E�����
				MapChip chip = mapChips[y][x];
				float chipTop = chip.m_pos.y;
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize;
				float chipRight = chip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = chip.m_pos.x + MapConsts::kMapOffsetX;

				// ��΂ɓ������Ă��Ȃ��ꍇcontinue
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top)    continue;
				if (chipRight < rect.left)   continue;
				if (chipLeft > rect.right)  continue;

				// ���������}�b�v�`�b�v�̋�`��ݒ肷��
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

bool Map::IsDamageCol(Rect rect, Rect& chipRect, std::weak_ptr<Camera> camera)
{
	// �}�b�v�̓����蔻��
	for (int y = 0; y < MapConsts::kMapHeight; y++)
	{
		for (int x = 0; x < MapConsts::kMapWidth; x++)
		{
			// �����蔻��������̂����肷��
			// WhiteList�ɓV��⏰�A�ǂȂ�...
			for (int i = 0; i < _countof(MapConsts::kDamageList); i++)
			{
				if (mapChips[y][x].chipNo != MapConsts::kDamageList[i]) continue;

				// �J�����ɉ����ĕ␳
				Vec3 camOffset = camera.lock()->GetDrawOffset();

				// �����蔻�肵������̏㉺���E�����
				MapChip chip = mapChips[y][x];
				float chipBottom = chip.m_pos.y + MapConsts::kMapChipSize;
				float chipTop = chip.m_pos.y;
				float chipRight = chip.m_pos.x + MapConsts::kMapOffsetX + MapConsts::kMapChipSize;
				float chipLeft = chip.m_pos.x + MapConsts::kMapOffsetX;

				// ��΂ɓ������Ă��Ȃ��ꍇcontinue
				if (chipTop > rect.bottom) continue;
				if (chipBottom < rect.top)    continue;
				if (chipRight < rect.left)   continue;
				if (chipLeft > rect.right)  continue;

				// ���������}�b�v�`�b�v�̋�`��ݒ肷��
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
