// --------------------------------------------------------------------------------------
// File: Monster.h
//
// モンスター管理クラス
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

namespace PakPakMonster
{
	class Monster
	{
	public:

		enum class Type
		{
			Type1,	// 追いかけモンスター
			Type2,	// 気まぐれモンスター
		};

		enum class Anime
		{
			Normal_01,	// 通常パターン１
			Normal_02,	// 通常パターン２
			Return_01,	// 巣に戻るパターン１
			Return_02,	// 巣に戻るパターン２
			Return_03,	// 巣に戻るパターン３（目だけ）
		};

		enum class State
		{
			Normal,	// 通常
			Weak,	// 逃げてる
			Dead,	// 食べられた
			Eat,	// プレイヤーを食べた
			Return,	// 巣に戻っている
			Revive,	// 復活
		};

	private:

		// モンスターの移動速度の種類
		enum class Speed
		{
			Lv1,	// LV1（遅い）
			Lv2,	// LV2   ↓
			Lv3,	// LV3   ↓
			Lv4,	// LV4   ↓
			Lv5,	// LV5（速い）
			Weak,	// モンスターが弱っている時
			Return,	// モンスターが巣に戻る時
		};

		// モンスターの移動速度
		static const int SPEED[];

		// モンスターの構造体
		struct MonsterInfo
		{
			Type type;		// モンスタータイプ
			State state;	// 状態
			int weakTimer;	// 弱りタイマー
			Anime animeNo;	// アニメーションNo
			int animeTimer;	// アニメーション用タイマー
			int x, y;		// 位置
			int vx, vy;		// 速度
			int dir;		// 進行方向
		};

		// モンスター情報
		MonsterInfo m_monster[2];

		// モンスターのスタート位置
		static const int START_POS[2][2];

		// アニメーション表示時間（食べられて目になる）
		static const int ANIME_INTERVAL_DEAD[];

		// アニメーション表示時間（復活する）
		static const int ANIME_INTERVAL_REVIVE[];

	private:

		// モンスターの状態とレベルに応じた速度を取得する関数
		int GetSpeed(int type, int stage, int level);

		// モンスターの速度設定関数
		void SetVelocity(int type, int stage, int level);

		// 逆方向を取得する関数
		int GetReverseDir(int dir);

		// モンスターの復活地点にいるかチェックする関数
		bool CheckNestPos(int type);

		// モンスターの移動方向の取得
		int GetDir(int no);

		// 移動できる方向の数を取得する関数
		int GetDirCnt(int moveFlag);

		// ターゲットを追いかける
		int ChaseTarget(int moveFlag, int targetX, int targetY);

	public:

		// コンストラクタ
		Monster();

		// 初期化
		void Initialize();

		// 更新
		void Update(int stage, int level, int playerX, int playerY);

		// モンスターが食べられたり、復活するときの絵のパターンチェンジする関数
		bool ChangeBody(int type, int stage, int level);

		// モンスターが食べられた時呼ばれる関数
		void Dead(int type);

		// アニメーションの更新関数
		void UpdateAnime();

		// モンスターの状態を設定する関数
		void SetState(int type, Monster::State state);

		// モンスターの状態を取得する関数
		Monster::State GetState(int type);

		// モンスターの弱っている時間の残り時間を取得する関数
		int GetWeakTime(int type);

		// アニメーションの番号を取得関数
		Monster::Anime GetAnimeNo(int type);

		// 位置を取得する関数
		void GetPos(int type, int* x, int* y);

		// モンスターが弱くなる関数
		void WeakMonster();

	};

	inline void Monster::GetPos(int type, int* x, int* y)
	{
		// 8ドット単位の移動にする
		*x = (m_monster[type].x >> 7) << 3;
		*y = (m_monster[type].y >> 7) << 3;
	}
}

