// --------------------------------------------------------------------------------------
// File: Map.h
//
// �}�b�v�N���X
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Common.h"

class Map
{
public:

	// �����̏��
	enum class FoodState
	{
		None,
		Food_3,
		Food_2,
		Food_1,
		Power_3,
		Power_2,
		Power_1,
	};

	// �����̍\����
	struct FoodInfo
	{
		FoodState state;	// �����̏�� 
		int x, y;			// �����̈ʒu
	};

	// �{�[�i�X�̏��
	enum class BonusState
	{
		Wait,		// �\���҂�
		Display,	// �\����
		Score,		// �_���\����
	};

	// �{�[�i�X�����̍\����
	struct BonusInfo
	{
		BonusState state;	// ���
		int x, y;			// �ʒu
		int score;			// ���_
		int timer;			// �^�C�}�[

		void SetState(BonusState bonusState)
		{
			state = bonusState;
			switch (bonusState)
			{
			case BonusState::Display:
				timer = BONUS_DISPLAY_TIME;				// �{�[�i�X�̕\�����Ԃ�ݒ�
				break;
			case BonusState::Score:
				timer = BONUS_SCORE_DISPLAY_TIME;		// �{�[�i�X�̕\�����Ԃ�ݒ�
				break;
			case BonusState::Wait:
				timer = (15 + rand() % (5 + 1)) * 60;	// ���̃{�[�i�X���ł�܂ł̎��Ԃ�ݒ�
				break;
			default:
				break;
			}
		}
	};

private:

	// �Q�[�����Ɏg����}�b�v�f�[�^�ւ̃|�C���^
	const int(*m_foodMap)[MAP_SIZE];

	// �����̏��
	FoodInfo m_foodInfo[FOOD_MAX];

	// �{�[�i�X�����̏��
	BonusInfo m_bonusInfo;

public:

	// �R���X�g���N�^
	Map();

	// �����̏�����
	void InitiaizeFood();

	// �}�b�v�̏�����
	void InitializeMap();

	// �{�[�i�X�����̏�����
	void InitializeBonus();

	// �w��ʒu�̂������̎擾
	FoodInfo* GetFoodInfo(int x, int y);

	// �X�e�[�W��̂����̐����擾����֐�
	int GetFoodCnt();

	// �}�b�v�̍X�V
	void Update();

	// �����̏����擾����֐�
	const FoodInfo* GetFoodInfo(int no) { return &m_foodInfo[no]; }

	// �{�[�i�X�̏����擾����֐�
	BonusInfo* GetBonusInfo() { return &m_bonusInfo; }

};

