/ --------------------------------------------------------------------------------------
// File: Player.cpp
//
// �v���C���[�N���X
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "../pch.h"
#include "Player.h"
#include "Direction.h"
#include "Common.h"

using namespace DirectX;
using namespace PakPakMonster;

// �R���X�g���N�^
Player::Player()
	: m_state(PlayerState::Stop)
	, m_x(0), m_y(0)
	, m_vx(0), m_vy(0)
	, m_dir(0)
	, m_left(false)
	, m_foodX(0), m_foodY(0)
	, m_powerupTimer(0)
	, m_animeId(0)
	, m_animeTimer(0)
{
}

// ������
void Player::Initialize()
{
	m_state = PlayerState::Stop;
	m_x = (3 * CHIP_SIZE) << 4;
	m_y = (5 * CHIP_SIZE) << 4;
	m_dir = RIGHT;
	SetVelocity();
	m_left = false;	// �E����
	m_foodX = m_foodY = 0;
	m_powerupTimer = 0;
	m_animeTimer = 0;
	m_animeId = static_cast<int>(PlayerAnime::Normal_01);
}

// �X�V
void Player::Update(Map& map, int* score, bool* eatPowerup)
{
	auto key = Keyboard::Get().GetState();

	// �ړ��p�}�b�v�f�[�^
	const int (*moveMap)[MAP_SIZE] = MOVE_MAP;

	// �܂��p���[������H�ׂĂ��Ȃ�
	*eatPowerup = false;

	// �p���[������A���ŐH�ׂ��Ȃ��悤�ɂ���^�C�}�[�����Z����
	if (m_powerupTimer > 0) m_powerupTimer--;

	// �v���C���[���ړ�����
	SetVelocity();
	m_x += m_vx;
	m_y += m_vy;

	// �����]���ł���ꏊ���`�F�b�N�I�i���U�S�h�b�g�̃O���b�h�ɂ҂����肠�����ꏊ�j
	if (((m_vx == 0) || (m_x % (CHIP_SIZE << 4)) < abs(m_vx))
		&& ((m_vy == 0) || (m_y % (CHIP_SIZE << 4)) < abs(m_vy)))
	{
		int px = m_x >> 4;
		int py = m_y >> 4;
		int map_x = px / CHIP_SIZE;
		int map_y = py / CHIP_SIZE;
		int moveFlag = moveMap[map_y][map_x];

		// �i�s�������ړ��s�Ȃ�~�߂�
		if (!(moveFlag & UP) && m_vy < 0) m_dir = 0;
		if (!(moveFlag & DOWN) && m_vy > 0) m_dir = 0;
		if (!(moveFlag & LEFT) && m_vx < 0) m_dir = 0;
		if (!(moveFlag & RIGHT) && m_vx > 0) m_dir = 0;

		// �����]��
		if ((moveFlag & UP) && (key.Up)) m_dir = UP;
		if ((moveFlag & DOWN) && (key.Down)) m_dir = DOWN;
		if ((moveFlag & LEFT) && (key.Left)) m_dir = LEFT;
		if ((moveFlag & RIGHT) && (key.Right)) m_dir = RIGHT;

		// ����������ΐH�ׂ�
		Map::FoodInfo* food = map.GetFoodInfo(map_x, map_y);

		// ����������H
		if (food != nullptr && food->state != Map::FoodState::None)
		{
			// �p���[�����̏�H
			if ( food->state == Map::FoodState::Power_3
			  || food->state == Map::FoodState::Power_2
			  || food->state == Map::FoodState::Power_1 )
			{
				// �p���[������H�ׂ����ԁH
				if (m_powerupTimer == 0)
				{
					// �����ꏊ�̂����͎��Ȃ��̂Ŏ�����ꏊ���L�^���Ă���
					// �������p���[�����͐H�ׂ���
					m_foodX = map_x;
					m_foodY = map_y;

					// �p���[������H�ׂ�
					*eatPowerup = true;

					// ���_���Z
					*score += SCORE_POWER;

					// �p���[�A�b�v�̎��Ԃ�ݒ�
					m_powerupTimer = POWERUP_TIME;

					// �����̏�Ԃ�ς���
					switch (food->state)
					{
					case Map::FoodState::Power_3:
						food->state = Map::FoodState::Power_2;
						break;
					case Map::FoodState::Power_2:
						food->state = Map::FoodState::Power_1;
						break;
					case Map::FoodState::Power_1:
						food->state = Map::FoodState::None;
						break;
					}
				}
			}
			else
			{
				// �O��H�ׂ������H
				if (m_foodX != map_x || m_foodY != map_y)
				{
					// �����ꏊ�̂����͎��Ȃ��̂Ŏ�����ꏊ���L�^���Ă���
					m_foodX = map_x;
					m_foodY = map_y;

					switch (food->state)
					{
					case Map::FoodState::Food_3:
						food->state = Map::FoodState::Food_2;
						break;
					case Map::FoodState::Food_2:
						food->state = Map::FoodState::Food_1;
						break;
					case Map::FoodState::Food_1:
						food->state = Map::FoodState::None;
						break;
					}
					// ���_���Z
					*score += SCORE_FOOD;
				}
			}
		}
		else
		{
			// �������Ō�̂P�Ȃ̂ŏ�ɍŌ�ɐH�ׂ��ʒu���X�V���ĘA���ŐH�ׂ���悤�ɂ���
			if (map.GetFoodCnt() == 1)
			{
				m_foodX = map_x;
				m_foodY = map_y;
			}
		}
		// �H�擾���`�F�b�N
		Map::BonusInfo* bonus = map.GetBonusInfo();
		if ( (map_x == 1 || map_x == 5) && (map_y == 3)	// �H�̈ʒu�ɂ���
		   && map_x == (bonus->x / CHIP_SIZE)		// �H������
		   && bonus->state == Map::BonusState::Display)	// �H���擾�ł�����
		{
			bonus->SetState(Map::BonusState::Score);
			// ���_���Z
			*score += bonus->score;
		}
	}
	else
	{
		// �㉺�Ɉړ����͏㉺�݈̂ړ��\
		if (m_vy != 0)
		{
			if (key.Up) m_dir = UP;
			if (key.Down) m_dir = DOWN;
		}

		// ���E�Ɉړ����͍��E�݈̂ړ��\�i�������ς��Ă���j
		if (m_vx != 0)
		{
			if (key.Left) m_dir = LEFT;
			if (key.Right) m_dir = RIGHT;
		}
	}
}

// ���S
void Player::Dead()
{
	m_state = PlayerState::Dead;
	m_animeTimer = 0;
}

// �A�j���[�V�����̍X�V�֐�
void Player::UpdateAnime()
{
	PlayerAnime animeNo[8] =
	{
		PlayerAnime::Normal_01,
		PlayerAnime::Dead_01,
		PlayerAnime::Dead_02,
		PlayerAnime::Dead_03,
		PlayerAnime::None,
		PlayerAnime::Dead_03,
		PlayerAnime::Dead_02,
		PlayerAnime::Dead_01,
	};

	// �v���C���[�����񂾎��Ȃ�
	if (m_state == Player::PlayerState::Dead)
	{
		int timer = m_animeTimer >> 1;
		if (timer < 8 * 9)
		{
			m_animeTimer++;
			if (timer > 8 * 3) m_animeId = timer % 8;
		}
		else {
			m_animeId = static_cast<int>(PlayerAnime::None);
		}
	}

	// �ʏ펞�̓p�N�p�N
	if (m_state == Player::PlayerState::Normal)
	{
		m_animeTimer++;
		if (m_animeTimer == 8)
		{
			m_animeTimer = 0;
			if (m_animeId == static_cast<int>(PlayerAnime::Normal_01))
			{
				m_animeId = static_cast<int>(PlayerAnime::Normal_02);
			}
			else
			{
				m_animeId = static_cast<int>(PlayerAnime::Normal_01);
			}
		}
	}
}

// �����Ă���������瑬�x��ݒ肷��֐�
void Player::SetVelocity()
{
	m_vx = m_vy = 0;

	switch (m_dir)
	{
	case UP:
		m_vx = 0;
		m_vy = -PLAYER_SPEED;
		break;
	case DOWN:
		m_vx = 0;
		m_vy = PLAYER_SPEED;
		break;
	case LEFT:
		m_left = true;	// ������
		m_vx = -PLAYER_SPEED;
		m_vy = 0;
		break;
	case RIGHT:
		m_left = false;	// �E����
		m_vx = PLAYER_SPEED;
		m_vy = 0;
		break;
	}
}
