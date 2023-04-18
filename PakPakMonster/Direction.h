// -------------------------------------------------------------------------------------
// File: Direction.h
//
// •ûŒü‚Ì’è”‚ğ‚Ü‚Æ‚ß‚½ƒwƒbƒ_
//
// Date: 2023.4.17
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

namespace PakPakMonster
{
	// ˆÚ“®•ûŒü
	static const int UP = (1 << 0);
	static const int DOWN = (1 << 1);
	static const int LEFT = (1 << 2);
	static const int RIGHT = (1 << 3);

	static const int U = UP;
	static const int D = DOWN;
	static const int L = LEFT;
	static const int R = RIGHT;

	static const int DR = (D | R);
	static const int DL = (D | L);
	static const int UR = (U | R);
	static const int UL = (U | L);
	static const int LR = (L | R);
	static const int UD = (U | D);
	static const int UDR = (U | D | R);
	static const int UDL = (U | D | L);
	static const int ULR = (U | L | R);
	static const int DLR = (D | L | R);
	static const int UDLR = (U | D | L | R);
}

