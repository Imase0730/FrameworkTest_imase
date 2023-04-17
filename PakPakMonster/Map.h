#pragma once

#include "Common.h"

class Map
{
public:

	// えさの状態
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

	// えさの構造体
	struct FoodInfo
	{
		FoodState state;	// えさの状態 
		int x, y;			// えさの位置
	};

	// ボーナスの状態
	enum class BonusState
	{
		Wait,		// 表示待ち
		Display,	// 表示中
		Score,		// 点数表示中
	};

	// ボーナスえさの構造体
	struct BonusInfo
	{
		BonusState state;	// 状態
		int x, y;			// 位置
		int score;			// 得点
		int timer;			// タイマー

		void SetState(BonusState bonusState)
		{
			state = bonusState;
			switch (bonusState)
			{
			case BonusState::Display:
				timer = BONUS_DISPLAY_TIME;				// ボーナスの表示時間を設定
				break;
			case BonusState::Score:
				timer = BONUS_SCORE_DISPLAY_TIME;		// ボーナスの表示時間を設定
				break;
			case BonusState::Wait:
				timer = (15 + rand() % (5 + 1)) * 60;	// 次のボーナスがでるまでの時間を設定
				break;
			default:
				break;
			}
		}
	};

private:

	// ゲーム中に使われるマップデータへのポインタ
	const int(*m_foodMap)[MAP_SIZE];

	// えさの情報
	FoodInfo m_foodInfo[FOOD_MAX];

	// ボーナスえさの情報
	BonusInfo m_bonusInfo;

public:

	// コンストラクタ
	Map();

	// えさの初期化
	void InitiaizeFood();

	// マップの初期化
	void InitializeMap();

	// ボーナスえさの初期化
	void InitializeBonus();

	// 指定位置のえさ情報の取得
	FoodInfo* GetFoodInfo(int x, int y);

	// ステージ上のえさの数を取得する関数
	int GetFoodCnt();

	// マップの更新
	void Update();

	// えさの情報を取得する関数
	const FoodInfo* GetFoodInfo(int no) { return &m_foodInfo[no]; }

	// ボーナスの情報を取得する関数
	BonusInfo* GetBonusInfo() { return &m_bonusInfo; }

};

