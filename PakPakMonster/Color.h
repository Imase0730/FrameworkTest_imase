// --------------------------------------------------------------------------------------
// File: Colorn.h
//
// �g�p����F�萔���܂Ƃ߂��w�b�_
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

// �F
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

// �F�e�[�u��
static const int s_color[COLOR_MAX][3] =
{
	{   0,   0,   0 },	// ��
	{ 193, 255,  73 },	// ��
	{ 250, 169, 240 },	// �s���N
	{ 255, 213, 157 },	// �I�����W
	{ 178, 233, 255 },	// ���F
	{ 215, 174, 255 },	// ��
	{ 224, 227, 196 },	// �O���[
	{ 255, 209,  75 },	// ���F
};
