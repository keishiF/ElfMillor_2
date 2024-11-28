#include "Map.h" 
#include "game.h"
#include "DxLib.h"
#include <cassert>

namespace
{
	// �}�b�v�`�b�v1�̃T�C�Y
	constexpr int kMapChipWidth = 32;
	constexpr int kMapChipHeight = 32;

	// �p�[�c�̑I��͈�
	constexpr int kMapPartsNumX= 46;
	constexpr int kMapPartsNumY = 28;

	// �}�b�v�`�b�v�̗v�f��(�傫��)
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
	// �O���t�B�b�N�̓ǂݍ���
	m_handle = LoadGraph("img/BackGround/Map.jpg");
	assert(m_handle != -1);

	// fmf�t�@�C���̓ǂݍ���
	std::ifstream ifs(kFileName);
	// �ǂݍ��߂Ă��Ȃ��Ƃ��̃G���[�\��
	if (!ifs)
	{
		std::cerr << "Error:file not opened." << std::endl;
	}

	// �T�C�Y�𒲂ׂ�
	ifs.seekg(0, std::ios::end);
	int size = ifs.tellg();
	ifs.seekg(0);

	//�Ȃ����ŏ���20�o�C�g�����ʂȃf�[�^�ɂȂ��Ă���̂�
	//20�o�C�g���̖��ʂȃf�[�^��ǂݔ�΂�
	for (int i = 0; i < 20; i++)
	{
		char tmp;
		ifs.read(&tmp, sizeof(char));
	}

	// �f�[�^��ǂݍ���
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

	// �f�[�^��\��
	for (int i = 0; i < data.size(); i++)
	{
		std::cout << "�z��ԍ�" << i << " : " << static_cast<int>(data[i]) << std::endl;
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
				// continue�͌J��Ԃ���(for,while)�̒��Ŏg�p���閽��
				// continue���Ă΂ꂽ��ȍ~�̌J��Ԃ������͍s�킸���̃��[�v�Ɉڍs����
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