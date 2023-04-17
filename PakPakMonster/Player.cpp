#include "../pch.h"
#include "Player.h"
#include "Direction.h"
#include "Common.h"

using namespace DirectX;

// コンストラクタ
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

// 初期化
void Player::Initialize()
{
	m_state = PlayerState::Stop;
	m_x = (3 * CHIP_SIZE) << 4;
	m_y = (5 * CHIP_SIZE) << 4;
	m_dir = RIGHT;
	SetVelocity();
	m_left = false;	// 右向き
	m_foodX = m_foodY = 0;
	m_powerupTimer = 0;
	m_animeTimer = 0;
	m_animeId = static_cast<int>(PlayerAnime::Normal_01);
}

// 更新
void Player::Update(Map& map, int* score, bool* eatPowerup)
{
	auto key = Keyboard::Get().GetState();

	// 移動用マップデータ
	const int (*moveMap)[MAP_SIZE] = MOVE_MAP;

	// まだパワーえさを食べていない
	*eatPowerup = false;

	// パワーえさを連続で食べれれないようにするタイマーを減算する
	if (m_powerupTimer > 0) m_powerupTimer--;

	// プレイヤーを移動する
	SetVelocity();
	m_x += m_vx;
	m_y += m_vy;

	// 方向転換できる場所かチェック！（幅６４ドットのグリッドにぴったりあった場所）
	if (((m_vx == 0) || (m_x % (CHIP_SIZE << 4)) < abs(m_vx))
		&& ((m_vy == 0) || (m_y % (CHIP_SIZE << 4)) < abs(m_vy)))
	{
		int px = m_x >> 4;
		int py = m_y >> 4;
		int map_x = px / CHIP_SIZE;
		int map_y = py / CHIP_SIZE;
		int moveFlag = moveMap[map_y][map_x];

		// 進行方向が移動不可なら止める
		if (!(moveFlag & UP) && m_vy < 0) m_dir = 0;
		if (!(moveFlag & DOWN) && m_vy > 0) m_dir = 0;
		if (!(moveFlag & LEFT) && m_vx < 0) m_dir = 0;
		if (!(moveFlag & RIGHT) && m_vx > 0) m_dir = 0;

		// 方向転換
		if ((moveFlag & UP) && (key.Up)) m_dir = UP;
		if ((moveFlag & DOWN) && (key.Down)) m_dir = DOWN;
		if ((moveFlag & LEFT) && (key.Left)) m_dir = LEFT;
		if ((moveFlag & RIGHT) && (key.Right)) m_dir = RIGHT;

		// えさがあれば食べる
		Map::FoodInfo* food = map.GetFoodInfo(map_x, map_y);

		// えさがある？
		if (food != nullptr && food->state != Map::FoodState::None)
		{
			// 前回食べたえさ？
			if (m_foodX != map_x || m_foodY != map_y)
			{
				// 同じ場所のえさは取れないので取った場所を記録しておく
				m_foodX = map_x;
				m_foodY = map_y;

				// パワーえさの上？
				if ( food->state == Map::FoodState::Power_3
				  || food->state == Map::FoodState::Power_2
				  || food->state == Map::FoodState::Power_1 )
				{
					// パワーえさを食べられる状態？
					if (m_powerupTimer == 0)
					{
						// パワーえさを食べた
						*eatPowerup = true;

						// 得点加算
						*score += SCORE_POWER;

						// パワーアップの時間を設定
						m_powerupTimer = POWERUP_TIME;

						// えさの状態を変える
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
					// 得点加算
					*score += SCORE_FOOD;
				}
			}
		}
		else
		{
			// えさが最後の１つなので常に最後に食べた位置を更新して連続で食べられるようにする
			if (map.GetFoodCnt() == 1)
			{
				m_foodX = map_x;
				m_foodY = map_y;
			}
		}
		// ？取得をチェック
		Map::BonusInfo* bonus = map.GetBonusInfo();
		if ( (map_x == 1 || map_x == 5) && (map_y == 3)	// ？の位置にいる
		   && map_x == (bonus->x / CHIP_SIZE)		// ？がある
		   && bonus->state == Map::BonusState::Display)	// ？が取得できる状態
		{
			bonus->SetState(Map::BonusState::Score);
			// 得点加算
			*score += bonus->score;
		}
	}
	else
	{
		// 上下に移動中は上下のみ移動可能
		if (m_vy != 0)
		{
			if (key.Up) m_dir = UP;
			if (key.Down) m_dir = DOWN;
		}

		// 左右に移動中は左右のみ移動可能（向きも変えている）
		if (m_vx != 0)
		{
			if (key.Left) m_dir = LEFT;
			if (key.Right) m_dir = RIGHT;
		}
	}
}

// 死亡
void Player::Dead()
{
	m_state = PlayerState::Dead;
	m_animeTimer = 0;
}

// アニメーションの更新関数
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

	// プレイヤーが死んだ時なら
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

	// 通常時はパクパク
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

// 向いている方向から速度を設定する関数
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
		m_left = true;	// 左向き
		m_vx = -PLAYER_SPEED;
		m_vy = 0;
		break;
	case RIGHT:
		m_left = false;	// 右向き
		m_vx = PLAYER_SPEED;
		m_vy = 0;
		break;
	}
}
