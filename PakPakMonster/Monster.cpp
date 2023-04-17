// --------------------------------------------------------------------------------------
// File: Monster.cpp
//
// モンスター管理クラス
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "../pch.h"
#include "Monster.h"
#include "Direction.h"
#include "Common.h"

using namespace PakPakMonster;

// モンスターのスタート地点
const int Monster::START_POS[MONSTER_CNT][2] =
{
	(3 * CHIP_SIZE) << 4, (2 * CHIP_SIZE + 8) << 4,	// 追いかけモンスター
	(3 * CHIP_SIZE) << 4, (4 * CHIP_SIZE - 8) << 4,	// 気まぐれモンスター
};

// モンスターの移動速度
const int Monster::SPEED[] =
{
	12, 18, 24, 30, 36, 12, 32,
};

// アニメーション表示時間（食べられて目になる）
const int Monster::ANIME_INTERVAL_DEAD[] = { 16, 16, 16, 16, 40 };

// アニメーション表示時間（復活する）
const int Monster::ANIME_INTERVAL_REVIVE[] = { 32, 32, 32, 32, 40 };

// コンストラクタ
Monster::Monster()
	: m_monster{}
{
}

void Monster::Initialize()
{
	// 各変数の初期化
	for (int i = 0; i < 2; i++)
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
	// 追いかけモンスター
	m_monster[0].type = Type::Type1;
	// 気まぐれモンスター
	m_monster[1].type = Type::Type2;
}

void Monster::Update(int stage, int level, int playerX, int playerY)
{
	// 移動用マップデータ
	const int(*moveMap)[MAP_SIZE] = MOVE_MAP;

	// モンスター用移動＆戻り用マップデータ
	const int(*monsterMap)[MAP_SIZE] = MONSTER_MAP;

	for (int i = 0; i < 2; i++)
	{
		// 復活
		if (m_monster[i].state == State::Revive)
		{
			ChangeBody(i, stage, level);
			continue;
		}

		// モンスターが弱っている時間を減算する
		if (m_monster[i].weakTimer > 0)
		{
			m_monster[i].weakTimer--;
			// モンスターは通常状態に戻る
			if (m_monster[i].weakTimer == 0) m_monster[i].state = State::Normal;
		}

		// モンスターの移動を移動する
		SetVelocity(i, stage, level);
		m_monster[i].x += m_monster[i].vx;
		m_monster[i].y += m_monster[i].vy;

		// モンスターが巣に到着したかチェック
		if (m_monster[i].state == State::Return && CheckNestPos(i) == true)
		{
			// 復活モード
			m_monster[i].state = State::Revive;
			m_monster[i].animeTimer = ANIME_INTERVAL_REVIVE[static_cast<int>(m_monster[i].animeNo)];
		}

		// 方向転換できる場所かチェック！（幅６４ドットのグリッドにぴったりあった場所）
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

			// 現在のモンスターの進行方向を取得
			int dir = GetDir(i);
			// 進行方向とは逆方向を取得する
			dir = GetReverseDir(dir);
			moveFlag &= ~dir;

			// 食べられて巣に戻るときなら
			if (m_monster[i].state == State::Return && monsterFlag != 0)
			{
				moveFlag = monsterFlag;
			}
			else
			{
				// 追いかけモンスターで通常状態なら
				if (i == static_cast<int>(Type::Type1) && m_monster[i].state == State::Normal)
				{
					// プレイヤーのいるエリアへ移動する
					moveFlag = ChaseTarget(moveFlag, playerX, playerY);
				}
				int dirCnt = GetDirCnt(moveFlag);
				// 方向を変えるか判断
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
			// 進行方向を設定
			m_monster[i].dir = moveFlag;
		}
	}
}

// モンスターの死んだ時と復活した時のアニメーション管理関数
bool Monster::ChangeBody(int type, int stage, int level)
{
	// タイマーが０なら通常時
	if (m_monster[type].animeTimer == 0) return true;

	// タイマーを減算する
	if (m_monster[type].animeTimer > 0) m_monster[type].animeTimer--;

	// タイマーが０になったらパターンを変える
	if (m_monster[type].animeTimer == 0)
	{
		// モンスターが死んだ場合
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
			// 表示時間設定
			m_monster[type].animeTimer = ANIME_INTERVAL_DEAD[static_cast<int>(m_monster[type].animeNo)];
		}
		else
		{
			// モンスター復活の場合
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
			// 表示時間設定
			m_monster[type].animeTimer = ANIME_INTERVAL_REVIVE[static_cast<int>(m_monster[type].animeNo)];
		}
	}
	return FALSE;
}

void Monster::Dead(int type)
{
	// モンスターは食べられた
	m_monster[type].state = State::Dead;
	m_monster[type].weakTimer = 0;
	m_monster[type].animeTimer = ANIME_INTERVAL_DEAD[static_cast<int>(m_monster[type].animeNo)];
}

// モンスターの状態とレベルに応じた速度を取得する関数
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

// モンスターの速度設定関数
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

// 進行方向とは逆方向を取得
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

// モンスターの復活地点にいるかチェックする関数
bool Monster::CheckNestPos(int type)
{
	if ( m_monster[type].x == START_POS[type][0]
	  && m_monster[type].y == START_POS[type][1] )
	{
		return true;
	}
	return false;
}

// モンスターの移動方向の取得
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

// 移動できる方向の数を取得する関数
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

// ターゲットを追いかける
int Monster::ChaseTarget(int moveFlag, int targetX, int targetY)
{
	// プレイヤーが４隅のどのエリアにいるか調べ、違うエリアにいる場合はそちらのエリアに移動する
	int px = targetX / (CHIP_SIZE * 4);
	int py = targetY / (CHIP_SIZE * 4);

	// モンスターの位置
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

	// 同じエリアなら何もしない
	if (dir == 0) dir = moveFlag;

	return dir;
}

// アニメーションの更新関数
void Monster::UpdateAnime()
{
	for (int i = 0; i < MONSTER_CNT; i++)
	{
		// 通常または弱っている場合は移動アニメーション
		if ((m_monster[i].state == Monster::State::Normal) || (m_monster[i].state == Monster::State::Weak))
		{
			int x = (m_monster[i].x >> 7) << 3;
			int y = (m_monster[i].y >> 7) << 3;
			m_monster[i].animeNo = static_cast<Anime>((x / 8 + y / 8) % 2);
		}

		// 食べられた時のアニメーション
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

// モンスターの状態を設定する関数
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


