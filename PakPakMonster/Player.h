//--------------------------------------------------------------------------------------
// File: Player.h
//
// �v���C���[�N���X
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Map.h"

namespace PakPakMonster
{
	class Player
	{
	private:

		// �v���C���[�̑���
		static const int PLAYER_SPEED = (2 << 4);

	public:

		enum class PlayerState
		{
			Normal,	// �ʏ�
			Dead,	// ����
			Stop,	// ��~
		};

		enum class PlayerAnime
		{
			Normal_01,	// �ʏ�p�^�[���P
			Normal_02,	// �ʏ�p�^�[���Q
			Dead_01,	// ���S�p�^�[���P
			Dead_02,	// ���S�p�^�[���Q
			Dead_03,	// ���S�p�^�[���R
			None,		// �\���Ȃ�
		};

		// ���
		PlayerState m_state;

		// �ʒu
		int m_x, m_y;

		// ���x
		int m_vx, m_vy;

		// �i�s����
		int m_dir;

		// true�̏ꍇ�͍�����
		bool m_left;

		// �H�ׂ������̏ꏊ
		int m_foodX, m_foodY;

		// �p���[�A�b�v�̎c�莞��
		int m_powerupTimer;

		// �A�j���[�V����ID
		int m_animeId;

		// �A�j���[�V�����p�^�C�}�[
		int m_animeTimer;

	private:

		// �����Ă���������瑬�x��ݒ肷��֐�
		void SetVelocity();

	public:

		// �R���X�g���N�^
		Player();

		// �v���C���[�̏�����
		void Initialize();

		// �v���C���[�̍X�V
		void Update(Map& map, int* score, bool* eatPowerup);

		// �v���C���[���H�ׂ�ꂽ���Ă΂��֐�
		void Dead();

		// �v���C���[�̏�Ԃ�ݒ肷��֐�
		void SetState(PlayerState state) { m_state = state; }

		// �v���C���[�̏�Ԃ��擾����֐�
		PlayerState GetState() { return m_state; }

		// �v���C���[�̃A�j���[�V����ID���擾����֐�
		int GetAnimeId() { return m_animeId; }

		// �v���C���[�̈ʒu���擾����֐�
		void GetPos(int* x, int* y);

		// �����������ׂ�֐�
		bool IsLeft() { return m_left; }

		// �A�j���[�V�����̍X�V�֐�
		void UpdateAnime();

		// �p���[�A�b�v�������ׂ�֐�
		bool IsPowerup();

	};

	// �v���C���[�̈ʒu���擾����֐�
	inline void Player::GetPos(int* x, int* y)
	{
		// 8�h�b�g�P�ʂ̈ړ��ɂ���
		*x = (m_x >> 7) << 3;
		*y = (m_y >> 7) << 3;
	}

	// �p���[�A�b�v�������ׂ�֐�
	inline bool Player::IsPowerup()
	{
		if (m_powerupTimer == 0) return false;
		return true;
	}
}
