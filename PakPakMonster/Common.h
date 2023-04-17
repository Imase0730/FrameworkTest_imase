// --------------------------------------------------------------------------------------
// File: Common.h
//
// ���ʂŎg�p����萔���܂Ƃ߂��w�b�_
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Direction.h"

namespace PakPakMonster
{
	// ��ʃT�C�Y
	static const int SCREEN_W = 456;
	static const int SCREEN_H = 440;

	// �}�b�v�`�b�v�̃T�C�Y
	static const int CHIP_SIZE = 64;

	// �}�b�v��̂����̐�
	static const int FOOD_MAX = 8;

	// �}�b�v�T�C�Y
	static const int MAP_SIZE = 7;

	// �����X�^�[�̐�
	static const int MONSTER_CNT = 2;

	// ���_
	static const int SCORE_FOOD = 20;		// ����
	static const int SCORE_POWER = 100;		// �p���[����
	static const int SCORE_MONSTER = 100;	// �����X�^�[

	// �c�@��
	static const int LEVEL1_PLAYER_CNT = 6;	// ���x���P�i�U�@�j
	static const int LEVEL2_PLAYER_CNT = 3;	// ���x���Q�i�R�@�j

	// �L�����N�^�[�T�C�Y
	static const int CHARACTER_SIZE = 56;

	// �p���[�����̌��ʎ��ԁi�T�b�j
	static const int POWERUP_TIME = (5 * 60);

	// �Q�[�����n�܂�܂ł̎��ԁi1.5�b�j
	static const int START_WAIT = 90;

	// �v���C���[�����S�������̑҂����ԁi�T�b�j
	static const int  PAUSE_TIME_DEAD = (5 * 60);

	// �����X�^�[��H�ׂ����̑҂����ԁi�R�b�j
	static const int  PAUSE_TIME_EAT = (3 * 60);

	// �X�e�[�W�N���A���̑҂����ԁi�R�b�j
	static const int  PAUSE_TIME_CLEAR = (3 * 60);

	// �Q�[���I�[�o�[���̑҂����ԁi�T�b�j
	static const int  PAUSE_TIME_GAMEOVER = (5 * 60);

	// �{�[�i�X�����\�����ԁi�T�b�j
	static const int BONUS_DISPLAY_TIME = (5 * 60);

	// �{�[�i�X�̓��_�\�����ԁi�S�b�j
	static const int BONUS_SCORE_DISPLAY_TIME = (4 * 60);

	// �Փ˔���̗]��
	static const int COLLISION_SPACE = 8;

	// �{�[�i�X�̎�ނ̐�
	static const int BONUS_KIND_MAX = 8;

	// �{�[�i�X�̓��_
	static const int BONUS_SCORE[BONUS_KIND_MAX] = { 100, 200, 300, 400, 500, 600, 700, 800 };

	// �ړ��\�}�b�v
	static const int MOVE_MAP[MAP_SIZE][MAP_SIZE] =
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
	static const int MONSTER_MAP[MAP_SIZE][MAP_SIZE] =
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
	static const int FOOD_MAP1[MAP_SIZE][MAP_SIZE] =
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
	static const int FOOD_MAP2[MAP_SIZE][MAP_SIZE] =
	{
		1, 0, 0, 0, 0, 0, 2,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 0, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 1,
	};
}

