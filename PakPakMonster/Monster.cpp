// -------------------------------------------------------------------------------------
// File: Monster.cpp
//
// �����X�^�[�Ǘ��N���X
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "../pch.h"
#include "Monster.h"
#include "Direction.h"
#include "Common.h"

using namespace PakPakMonster;

// �����X�^�[�̃X�^�[�g�n�_
const int Monster::START_POS[MONSTER_CNT][2] =
{
	(3 * CHIP_SIZE) << 4, (2 * CHIP_SIZE + 8) << 4,	// �ǂ����������X�^�[
	(3 * CHIP_SIZE) << 4, (4 * CHIP_SIZE - 8) << 4,	// �C�܂��ꃂ���X�^�[
};

// �����X�^�[�̈ړ����x
const int Monster::SPEED[] =
{
	12, 18, 24, 30, 36, 12, 32,
};

// �A�j���[�V�����\�����ԁi�H�ׂ��ĖڂɂȂ�j
const int Monster::ANIME_INTERVAL_DEAD[] = { 16, 16, 16, 16, 40 };

// �A�j���[�V�����\�����ԁi��������j
const int Monster::ANIME_INTERVAL_REVIVE[] = { 32, 32, 32, 32, 40 };

// �R���X�g���N�^
Monster::Monster()
	: m_monster{}
{
}

void Monster::Initialize()
{
	// �e�ϐ��̏�����
	for (int i = 0; i < MONSTER_CNT; i++)
	{
		m_monster[i].state = State::Normal;
		m_monster[i].dir = UP;
		m_monster[i].x = START_POS[i][0];
		m_monster[i].y = START_POS[i][1];
		m_monster[i].vx = m_monster[i].vy = 0;
		m_monster[i].weakTimer = 0;
		m_monster[i].animeNo = Anime::Normal_01;
		m_monster[i].animeTimer = 0;
	}
	// �ǂ����������X�^�[
	m_monster[0].type = Type::Type1;
	// �C�܂��ꃂ���X�^�[
	m_monster[1].type = Type::Type2;
}

void Monster::Update(int stage, int level, int playerX, int playerY)
{
	// �ړ��p�}�b�v�f�[�^
	const int(*moveMap)[MAP_SIZE] = MOVE_MAP;

	// �����X�^�[�p�ړ����߂�p�}�b�v�f�[�^
	const int(*monsterMap)[MAP_SIZE] = MONSTER_MAP;

	for (int i = 0; i < 2; i++)
	{
		// ����
		if (m_monster[i].state == State::Revive)
		{
			ChangeBody(i, stage, level);
			continue;
		}

		// �����X�^�[������Ă��鎞�Ԃ����Z����
		if (m_monster[i].weakTimer > 0)
		{
			m_monster[i].weakTimer--;
			// �����X�^�[�͒ʏ��Ԃɖ߂�
			if (m_monster[i].weakTimer == 0) m_monster[i].state = State::Normal;
		}

		// �����X�^�[�̈ړ����ړ�����
		SetVelocity(i, stage, level);
		m_monster[i].x += m_monster[i].vx;
		m_monster[i].y += m_monster[i].vy;

		// �����X�^�[�����ɓ����������`�F�b�N
		if (m_monster[i].state == State::Return && CheckNestPos(i) == true)
		{
			// �������[�h
			m_monster[i].state = State::Revive;
			m_monster[i].animeTimer = ANIME_INTERVAL_REVIVE[static_cast<int>(m_monster[i].animeNo)];
		}

		// �����]���ł���ꏊ���`�F�b�N�I�i���U�S�h�b�g�̃O���b�h�ɂ҂����肠�����ꏊ�j
		if (((m_monster[i].vx == 0) || (m_monster[i].x % (CHIP_SIZE << 4)) < abs(m_monster[i].vx))
			&& ((m_monster[i].vy == 0) || (m_monster[i].y % (CHIP_SIZE << 4)) < abs(m_monster[i].vy)))
		{
			int mx = m_monster[i].x >> 4;
			int my = m_monster[i].y >> 4;
			int map_x = mx / CHIP_SIZE;
			int map_y = my / CHIP_SIZE;

			m_monster[i].x = (map_x * CHIP_SIZE) << 4;
			m_monster[i].y = (map_y * CHIP_SIZE) << 4;

			int moveFlag = moveMap[map_y][map_x];
			int monsterFlag = monsterMap[map_y][map_x];

			// ���݂̃����X�^�[�̐i�s�������擾
			int dir = GetDir(i);
			// �i�s�����Ƃ͋t�������擾����
			dir = GetReverseDir(dir);
			moveFlag &= ~dir;

			// �H�ׂ��đ��ɖ߂�Ƃ��Ȃ�
			if (m_monster[i].state == State::Return && monsterFlag != 0)
			{
				moveFlag = monsterFlag;
			}
			else
			{
				// �ǂ����������X�^�[�Œʏ��ԂȂ�
				if (i == static_cast<int>(Type::Type1) && m_monster[i].state == State::Normal)
				{
					// �v���C���[�̂���G���A�ֈړ�����
					moveFlag = ChaseTarget(moveFlag, playerX, playerY);
				}
				int dirCnt = GetDirCnt(moveFlag);
				// ������ς��邩���f
				if (monsterFlag & UDLR)
				{
					int cnt = rand() % dirCnt;
					dir = 1;
					for (int j = 0; j < 4; j++)
					{
						if (moveFlag & dir) cnt--;
						if (cnt < 0)
						{
							moveFlag = dir;
							break;
						}
						dir <<= 1;
					}
				}
			}
			// �i�s������ݒ�
			m_monster[i].dir = moveFlag;
		}
	}
}

// �����X�^�[�̎��񂾎��ƕ����������̃A�j���[�V�����Ǘ��֐�
bool Monster::ChangeBody(int type, int stage, int level)
{
	// �^�C�}�[���O�Ȃ�ʏ펞
	if (m_monster[type].animeTimer == 0) return true;

	// �^�C�}�[�����Z����
	if (m_monster[type].animeTimer > 0) m_monster[type].animeTimer--;

	// �^�C�}�[���O�ɂȂ�����p�^�[����ς���
	if (m_monster[type].animeTimer == 0)
	{
		// �����X�^�[�����񂾏ꍇ
		if (m_monster[type].state == State::Dead)
		{
			switch (m_monster[type].animeNo)
			{
			case Anime::Normal_01:
			case Anime::Normal_02:
				m_monster[type].animeNo = Anime::Return_01;
				break;
			case Anime::Return_01:
				m_monster[type].animeNo = Anime::Return_02;
				break;
			case Anime::Return_02:
				m_monster[type].animeNo = Anime::Return_03;
				break;
			case Anime::Return_03:
				m_monster[type].state = State::Return;
				m_monster[type].animeTimer = 0;
				return true;
			default:
				break;
			}
			// �\�����Ԑݒ�
			m_monster[type].animeTimer = ANIME_INTERVAL_DEAD[static_cast<int>(m_monster[type].animeNo)];
		}
		else
		{
			// �����X�^�[�����̏ꍇ
			switch (m_monster[type].animeNo)
			{
			case Anime::Normal_01:
			case Anime::Normal_02:
				m_monster[type].state = State::Normal;
				m_monster[type].dir = UP;
				SetVelocity(type, stage, level);
				m_monster[type].animeTimer = 0;
				return true;
			case Anime::Return_01:
				m_monster[type].animeNo = Anime::Normal_02;
				break;
			case Anime::Return_02:
				m_monster[type].animeNo = Anime::Return_01;
				break;
			case Anime::Return_03:
				m_monster[type].animeNo = Anime::Return_02;
				break;
			default:
				break;
			}
			// �\�����Ԑݒ�
			m_monster[type].animeTimer = ANIME_INTERVAL_REVIVE[static_cast<int>(m_monster[type].animeNo)];
		}
	}
	return FALSE;
}

void Monster::Dead(int type)
{
	// �����X�^�[�͐H�ׂ�ꂽ
	m_monster[type].state = State::Dead;
	m_monster[type].weakTimer = 0;
	m_monster[type].animeTimer = ANIME_INTERVAL_DEAD[static_cast<int>(m_monster[type].animeNo)];
}

// �����X�^�[�̏�Ԃƃ��x���ɉ��������x���擾����֐�
int Monster::GetSpeed(int type, int stage, int level)
{
	if (stage > 3) stage = 3;

	if (m_monster[type].state == State::Weak) return SPEED[static_cast<int>(Speed::Weak)];
	if (m_monster[type].state == State::Return) return SPEED[static_cast<int>(Speed::Return)];
	if (m_monster[type].state == State::Normal)
	{
		return SPEED[stage + level];
	}

	return 0;
}

// �����X�^�[�̑��x�ݒ�֐�
void Monster::SetVelocity(int type, int stage, int level)
{
	switch (m_monster[type].dir)
	{
	case UP:
		m_monster[type].vx = 0;
		m_monster[type].vy = -GetSpeed(type, stage, level);
		break;
	case DOWN:
		m_monster[type].vx = 0;
		m_monster[type].vy = GetSpeed(type, stage, level);
		break;
	case LEFT:
		m_monster[type].vx = -GetSpeed(type, stage, level);
		m_monster[type].vy = 0;
		break;
	case RIGHT:
		m_monster[type].vx = GetSpeed(type, stage, level);
		m_monster[type].vy = 0;
		break;
	}
}

// �i�s�����Ƃ͋t�������擾
int Monster::GetReverseDir(int dir)
{
	switch (dir)
	{
	case UP:
		return DOWN;
	case DOWN:
		return UP;
	case RIGHT:
		return LEFT;
	case LEFT:
		return RIGHT;
	}
	return 0;
}

// �����X�^�[�̕����n�_�ɂ��邩�`�F�b�N����֐�
bool Monster::CheckNestPos(int type)
{
	if ( m_monster[type].x == START_POS[type][0]
	  && m_monster[type].y == START_POS[type][1] )
	{
		return true;
	}
	return false;
}

// �����X�^�[�̈ړ������̎擾
int Monster::GetDir(int no)
{
	if (m_monster[no].vx != 0)
	{
		if (m_monster[no].vx > 0) return RIGHT;
		return LEFT;
	}
	if (m_monster[no].vy != 0)
	{
		if (m_monster[no].vy > 0) return DOWN;
		return UP;
	}
	return 0;
}

// �ړ��ł�������̐����擾����֐�
int Monster::GetDirCnt(int moveFlag)
{
	int cnt = 0;

	for (int i = 0; i < 4; i++)
	{
		cnt += moveFlag & 1;
		moveFlag >>= 1;
	}
	return cnt;
}

// �^�[�Q�b�g��ǂ�������
int Monster::ChaseTarget(int moveFlag, int targetX, int targetY)
{
	// �v���C���[���S���̂ǂ̃G���A�ɂ��邩���ׁA�Ⴄ�G���A�ɂ���ꍇ�͂�����̃G���A�Ɉړ�����
	int px = targetX / (CHIP_SIZE * 4);
	int py = targetY / (CHIP_SIZE * 4);

	// �����X�^�[�̈ʒu
	int mx, my;
	GetPos(static_cast<int>(Type::Type1), &mx, &my);
	mx = mx / (CHIP_SIZE * 4);
	my = my / (CHIP_SIZE * 4);

	int move = 0;

	if (px < mx) move |= LEFT;
	if (px > mx) move |= RIGHT;
	if (py < my) move |= UP;
	if (py > my) move |= DOWN;

	int dir = moveFlag & move;

	// �����G���A�Ȃ牽�����Ȃ�
	if (dir == 0) dir = moveFlag;

	return dir;
}

// �A�j���[�V�����̍X�V�֐�
void Monster::UpdateAnime()
{
	for (int i = 0; i < MONSTER_CNT; i++)
	{
		// �ʏ�܂��͎���Ă���ꍇ�͈ړ��A�j���[�V����
		if ((m_monster[i].state == Monster::State::Normal) || (m_monster[i].state == Monster::State::Weak))
		{
			int x = (m_monster[i].x >> 7) << 3;
			int y = (m_monster[i].y >> 7) << 3;
			m_monster[i].animeNo = static_cast<Anime>((x / 8 + y / 8) % 2);
		}

		// �H�ׂ�ꂽ���̃A�j���[�V����
		if (m_monster[i].state == State::Eat)
		{
			int timer = m_monster[i].animeTimer >> 1;
			if (timer < 8 * 9)
			{
				m_monster[i].animeTimer++;
				if (timer > 8 * 3) m_monster[i].animeNo = static_cast<Anime>((timer >> 2) % 2);
			}
		}
	}
}

// �����X�^�[�̏�Ԃ�ݒ肷��֐�
void Monster::SetState(int type, Monster::State state)
{
	m_monster[type].state = state;
}

Monster::State Monster::GetState(int type)
{
	return m_monster[type].state;
}

int Monster::GetWeakTime(int type)
{
	return m_monster[type].weakTimer;
}

void Monster::WeakMonster()
{
	for (int i = 0; i < MONSTER_CNT; i++)
	{
		if (m_monster[i].state == State::Normal || m_monster[i].state == State::Weak)
		{
			m_monster[i].state = State::Weak;
			m_monster[i].weakTimer = POWERUP_TIME;
		}
	}
}

Monster::Anime Monster::GetAnimeNo(int type)
{
	return m_monster[type].animeNo;
}


