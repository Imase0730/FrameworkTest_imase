// -------------------------------------------------------------------------------------
// File: Common.h
//
// 共通で使用する定数をまとめたヘッダ
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Direction.h"

namespace PakPakMonster
{
	// 画面サイズ
	static const int SCREEN_W = 456;
	static const int SCREEN_H = 440;

	// マップチップのサイズ
	static const int CHIP_SIZE = 64;

	// マップ上のえさの数
	static const int FOOD_MAX = 8;

	// マップサイズ
	static const int MAP_SIZE = 7;

	// モンスターの数
	static const int MONSTER_CNT = 2;

	// 得点
	static const int SCORE_FOOD = 20;		// えさ
	static const int SCORE_POWER = 100;		// パワーえさ
	static const int SCORE_MONSTER = 200;	// モンスター

	// 残機数
	static const int LEVEL1_PLAYER_CNT = 6;	// レベル１（６機）
	static const int LEVEL2_PLAYER_CNT = 3;	// レベル２（３機）

	// キャラクターサイズ
	static const int CHARACTER_SIZE = 56;

	// パワーえさの効果時間（５秒）
	static const int POWERUP_TIME = (5 * 60);

	// ゲームが始まるまでの時間（1.5秒）
	static const int START_WAIT = 90;

	// プレイヤーが死亡した時の待ち時間（５秒）
	static const int  PAUSE_TIME_DEAD = (5 * 60);

	// モンスターを食べた時の待ち時間（３秒）
	static const int  PAUSE_TIME_EAT = (3 * 60);

	// ステージクリア時の待ち時間（３秒）
	static const int  PAUSE_TIME_CLEAR = (3 * 60);

	// ゲームオーバー時の待ち時間（５秒）
	static const int  PAUSE_TIME_GAMEOVER = (5 * 60);

	// ボーナスえさ表示時間（５秒）
	static const int BONUS_DISPLAY_TIME = (5 * 60);

	// ボーナスの得点表示時間（４秒）
	static const int BONUS_SCORE_DISPLAY_TIME = (4 * 60);

	// 衝突判定の余白
	static const int COLLISION_SPACE = 8;

	// ボーナスの種類の数
	static const int BONUS_KIND_MAX = 8;

	// ボーナスの得点
	static const int BONUS_SCORE[BONUS_KIND_MAX] = { 100, 200, 300, 400, 500, 600, 700, 800 };

	// 移動可能マップ
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

	// モンスター用移動＆戻り用マップ
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

	// えさの配置マップ１
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

	// えさの配置マップ２
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

