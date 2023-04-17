// --------------------------------------------------------------------------------------
// File: Map.cpp
//
// マップクラス
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "../pch.h"
#include "Map.h"
#include "Direction.h"
#include "Common.h"

using namespace DirectX;

// コンストラクタ
Map::Map()
	: m_foodMap(nullptr)
	, m_foodInfo{}
{
}

// マップの初期化
void Map::InitializeMap()
{
	// ランダムでステージを決める
	if (rand() % 2)
	{
		m_foodMap = FOOD_MAP1;
	}
	else
	{
		m_foodMap = FOOD_MAP2;
	}
}

// えさの初期化
void Map::InitiaizeFood()
{
	int cnt = 0;

	// えさの配置情報からえさを設置する
	for (int j = 0; j < MAP_SIZE; j++)
	{
		for (int i = 0; i < MAP_SIZE; i++)
		{
			// えさかパワーえさなら
			if (m_foodMap[j][i] != 0)
			{
				m_foodInfo[cnt].x = i;
				m_foodInfo[cnt].y = j;
				if (m_foodMap[j][i] == 1)
				{
					// えさ
					m_foodInfo[cnt].state = FoodState::Food_3;
				}
				else
				{
					// パワーえさ
					m_foodInfo[cnt].state = FoodState::Power_3;
				}
				if (cnt < FOOD_MAX - 1) cnt++;
			}
		}
	}
}

// ボーナスえさの初期化
void Map::InitializeBonus()
{
	m_bonusInfo.state = BonusState::Wait;
	m_bonusInfo.score = 0;
	m_bonusInfo.x = m_bonusInfo.y = -1;
	m_bonusInfo.timer = (9 + rand() % (3 + 1)) * 60;
}

// 指定位置のえさ情報の取得
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

// ステージ上のえさの数を取得する関数
int Map::GetFoodCnt()
{
	int cnt = 0;

	for (int i = 0; i < FOOD_MAX; i++)
	{
		if (m_foodInfo[i].state != FoodState::None) cnt++;
	}
	return cnt;
}

// マップの更新
void Map::Update()
{
	// ----- ボーナス表示の処理 -----//
	if (m_bonusInfo.timer > 0) m_bonusInfo.timer--;
	if (m_bonusInfo.timer == 0)
	{
		switch (m_bonusInfo.state)
		{
		case BonusState::Wait:	// ？を出す
			if (rand() % (1 + 1))
			{
				// 右側
				m_bonusInfo.x = 1 * CHIP_SIZE;
			}
			else
			{
				// 左側
				m_bonusInfo.x = 5 * CHIP_SIZE;
			}
			m_bonusInfo.y = 3 * CHIP_SIZE;
			// ボーナスの得点をランダムで決定
			m_bonusInfo.score = BONUS_SCORE[rand() % BONUS_KIND_MAX];
			m_bonusInfo.SetState(BonusState::Display);
			break;
		case BonusState::Display:	// ？を消す
		case BonusState::Score:		// 得点を消す
			m_bonusInfo.SetState(BonusState::Wait);
			break;
		}
	}
}
