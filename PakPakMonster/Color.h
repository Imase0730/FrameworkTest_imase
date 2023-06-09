// -------------------------------------------------------------------------------------
// File: Colorn.h
//
// 色定数をまとめたヘッダ
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

namespace PakPakMonster
{
	// 色
	enum
	{
		COLOR_BLACK,
		COLOR_GREEN,
		COLOR_PINK,
		COLOR_ORANGE,
		COLOR_CYAN,
		COLOR_PURPLE,
		COLOR_GRAY,
		COLOR_YELLOW,

		COLOR_MAX
	};

	// 色テーブル
	static const int s_color[COLOR_MAX][3] =
	{
		{   0,   0,   0 },	// 黒
		{ 193, 255,  73 },	// 緑
		{ 250, 169, 240 },	// ピンク
		{ 255, 213, 157 },	// オレンジ
		{ 178, 233, 255 },	// 水色
		{ 215, 174, 255 },	// 紫
		{ 224, 227, 196 },	// グレー
		{ 255, 209,  75 },	// 黄色
	};
}

