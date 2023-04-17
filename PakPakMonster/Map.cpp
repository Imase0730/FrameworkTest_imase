// --------------------------------------------------------------------------------------
// File: Map.cpp
//
// �}�b�v�N���X
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "../pch.h"
#include "Map.h"
#include "Direction.h"
#include "Common.h"

using namespace DirectX;
using namespace PakPakMonster;

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
		m_foodMap = FOOD_MAP1;
	}
	else
	{
		m_foodMap = FOOD_MAP2;
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
			m_bonusInfo.score = BONUS_SCORE[rand() % BONUS_KIND_MAX];
			m_bonusInfo.SetState(BonusState::Display);
			break;
		case BonusState::Display:	// �H������
		case BonusState::Score:		// ���_������
			m_bonusInfo.SetState(BonusState::Wait);
			break;
		}
	}
}
