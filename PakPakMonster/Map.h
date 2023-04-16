#pragma once

class Map
{
public:

	// �p���[�����̌��ʎ���
	static const int POWERUP_TIME = (5 * 60);

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

	// �}�b�v�`�b�v�̃T�C�Y�i�U�S�h�b�g�j
	static const int CHIP_SIZE = 64;

	// �����̐�
	static const int FOOD_MAX = 8;

	// �}�b�v�T�C�Y
	static const int MAP_SIZE = 7;

private:

	// ���_
	static const int FOOD_SCORE = 20;	// ����
	static const int POWER_SCORE = 100;	// �p���[����

	// �{�[�i�X�����\�����ԁi�T�b�j
	static const int BONUS_DISPLAY_TIME = (5 * 60);

	// �{�[�i�X�̓��_�\�����ԁi�S�b�j
	static const int BONUS_SCORE_DISPLAY_TIME = (4 * 60);

	// �{�[�i�X�����̓��_
	static const int BONUS_KIND_MAX = 8;
	static const int m_bonusScore[BONUS_KIND_MAX];

	// �ړ��\�}�b�v
	static const int m_moveMap[MAP_SIZE][MAP_SIZE];

	// �����X�^�[�p�ړ����߂�p�}�b�v
	static const int m_monsterMap[MAP_SIZE][MAP_SIZE];

	// �����̔z�u�}�b�v�P
	static const int m_foodMap1[MAP_SIZE][MAP_SIZE];

	// �����̔z�u�}�b�v�Q
	static const int m_foodMap2[MAP_SIZE][MAP_SIZE];

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

	// �ړ��p�}�b�v�f�[�^�擾�֐�
	const int(*GetMoveMap())[Map::MAP_SIZE] { return m_moveMap; }

	// �����X�^�[�p�ړ����߂�p�}�b�v�f�[�^�擾�֐�
	const int(*GetMonsterMap())[Map::MAP_SIZE] { return m_monsterMap; }

};

