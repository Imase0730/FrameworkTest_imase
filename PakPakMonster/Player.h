//--------------------------------------------------------------------------------------
// File: Player.h
//
// プレイヤークラス
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Map.h"

class Player
{
private:

	// プレイヤーの速さ
	static const int PLAYER_SPEED = (2 << 4);

public:

	enum class PlayerState
	{
		Normal,	// 通常
		Dead,	// 死んだ
		Stop,	// 停止
	};

	enum class PlayerAnime
	{
		Normal_01,	// 通常パターン１
		Normal_02,	// 通常パターン２
		Dead_01,	// 死亡パターン１
		Dead_02,	// 死亡パターン２
		Dead_03,	// 死亡パターン３
		None,		// 表示なし
	};

	// 状態
	PlayerState m_state;

	// 位置
	int m_x, m_y;

	// 速度
	int m_vx, m_vy;

	// 進行方向
	int m_dir;

	// trueの場合は左向き
	bool m_left;

	// 食べたえさの場所
	int m_foodX, m_foodY;

	// パワーアップの残り時間
	int m_powerupTimer;

	// アニメーションID
	int m_animeId;

	// アニメーション用タイマー
	int m_animeTimer;

private:

	// 向いている方向から速度を設定する関数
	void SetVelocity();

public:

	// コンストラクタ
	Player();

	// プレイヤーの初期化
	void Initialize();

	// プレイヤーの更新
	void Update(Map& map, int* score, bool* eatPowerup);

	// プレイヤーが食べられた時呼ばれる関数
	void Dead();

	// プレイヤーの状態を設定する関数
	void SetState(PlayerState state) { m_state = state; }

	// プレイヤーの状態を取得する関数
	PlayerState GetState() { return m_state; }

	// プレイヤーのアニメーションIDを取得する関数
	int GetAnimeId() { return m_animeId; }

	// プレイヤーの位置を取得する関数
	void GetPos(int* x, int* y);

	// 左向きか調べる関数
	bool IsLeft() { return m_left; }

	// アニメーションの更新関数
	void UpdateAnime();

	// パワーアップ中か調べる関数
	bool IsPowerup();

};

// プレイヤーの位置を取得する関数
inline void Player::GetPos(int* x, int* y)
{
	// 8ドット単位の移動にする
	*x = (m_x >> 7) << 3;
	*y = (m_y >> 7) << 3;
}

// パワーアップ中か調べる関数
inline bool Player::IsPowerup()
{
	if (m_powerupTimer == 0) return false;
	return true;
}
