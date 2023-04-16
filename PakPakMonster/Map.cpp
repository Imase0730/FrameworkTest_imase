#include "../pch.h"
#include "Map.h"
#include "Direction.h"

using namespace DirectX;

// �ړ��\�}�b�v
const int Map::m_moveMap[MAP_SIZE][MAP_SIZE] =
{
	DR,  LR,  DL,   0,  DR,  LR, DL,
	UD,   0, UDR,  LR, UDL,   0, UD,
	UR, DLR,  UL,  UD,  UR, DLR, UL,
	 0,  UD,   0,  UD,   0,  UD,  0,
	DR, ULR,  DL,  UD,  DR, ULR, DL,
	UD,   0, UDR,  LR, UDL,   0, UD,
	UR,  LR,  UL,   0,  UR,  LR, UL,
};

// �����X�^�[�p�ړ����߂�p�}�b�v
const int Map::m_monsterMap[MAP_SIZE][MAP_SIZE] =
{
	0, 0, 0, 0, 0, 0, 0,
	0, 0, R, D, L, 0, 0,
	0, R, 0, 0, 0, L, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, U, 0, 0, 0, U, 0,
	0, 0, U, 0, U, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
};

// �����̔z�u�}�b�v�P
const int Map::m_foodMap1[MAP_SIZE][MAP_SIZE] =
{
	2, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 1,
};

// �����̔z�u�}�b�v�Q
const int Map::m_foodMap2[MAP_SIZE][MAP_SIZE] =
{
	1, 0, 0, 0, 0, 0, 2,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 1,
};

// �{�[�i�X�����̓��_
const int Map::m_bonusScore[BONUS_KIND_MAX] = { 100, 200, 300, 400, 500, 600, 700, 800 };

// �R���X�g���N�^
Map::Map()
	: m_foodMap(nullptr)
	, m_foodInfo{}
{
}

// �}�b�v�̏�����
void Map::InitializeMap()
{
	// �����_���ŃX�e�[�W�����߂�
	if (rand() % 2)
	{
		m_foodMap = m_foodMap1;
	}
	else
	{
		m_foodMap = m_foodMap2;
	}
}

// �����̏�����
void Map::InitiaizeFood()
{
	int cnt = 0;

	// �����̔z�u��񂩂炦����ݒu����
	for (int j = 0; j < MAP_SIZE; j++)
	{
		for (int i = 0; i < MAP_SIZE; i++)
		{
			// �������p���[�����Ȃ�
			if (m_foodMap[j][i] != 0)
			{
				m_foodInfo[cnt].x = i;
				m_foodInfo[cnt].y = j;
				if (m_foodMap[j][i] == 1)
				{
					// ����
					m_foodInfo[cnt].state = FoodState::Food_3;
				}
				else
				{
					// �p���[����
					m_foodInfo[cnt].state = FoodState::Power_3;
				}
				if (cnt < FOOD_MAX - 1) cnt++;
			}
		}
	}
}

// �{�[�i�X�����̏�����
void Map::InitializeBonus()
{
	m_bonusInfo.state = BonusState::Wait;
	m_bonusInfo.score = 0;
	m_bonusInfo.x = m_bonusInfo.y = -1;
	m_bonusInfo.timer = (9 + rand() % (3 + 1)) * 60;
}

// �w��ʒu�̂������̎擾
Map::FoodInfo* Map::GetFoodInfo(int x, int y)
{
	for (int i = 0; i < FOOD_MAX; i++)
	{
		if (m_foodInfo[i].x == x && m_foodInfo[i].y == y)
		{
			return &m_foodInfo[i];
		}
	}
	return nullptr;
}

// �X�e�[�W��̂����̐����擾����֐�
int Map::GetFoodCnt()
{
	int cnt = 0;

	for (int i = 0; i < FOOD_MAX; i++)
	{
		if (m_foodInfo[i].state != FoodState::None) cnt++;
	}
	return cnt;
}

// �}�b�v�̍X�V
void Map::Update()
{
	// ----- �{�[�i�X�\���̏��� -----//
	if (m_bonusInfo.timer > 0) m_bonusInfo.timer--;
	if (m_bonusInfo.timer == 0)
	{
		switch (m_bonusInfo.state)
		{
		case BonusState::Wait:	// �H���o��
			if (rand() % (1 + 1))
			{
				// �E��
				m_bonusInfo.x = 1 * CHIP_SIZE;
			}
			else
			{
				// ����
				m_bonusInfo.x = 5 * CHIP_SIZE;
			}
			m_bonusInfo.y = 3 * CHIP_SIZE;
			// �{�[�i�X�̓��_�������_���Ō���
			m_bonusInfo.score = m_bonusScore[rand() % BONUS_KIND_MAX];
			m_bonusInfo.SetState(BonusState::Display);
			break;
		case BonusState::Display:	// �H������
		case BonusState::Score:		// ���_������
			m_bonusInfo.SetState(BonusState::Wait);
			break;
		}
	}
}
