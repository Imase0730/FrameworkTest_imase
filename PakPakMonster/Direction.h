// --------------------------------------------------------------------------------------
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
#define UP    (1 << 0)
#define DOWN  (1 << 1)
#define LEFT  (1 << 2)
#define RIGHT (1 << 3)

#define U UP
#define D DOWN
#define L LEFT
#define R RIGHT

#define DR (D | R)
#define DL (D | L)
#define UR (U | R)
#define UL (U | L)
#define LR (L | R)
#define UD (U | D)
#define UDR (U | D | R)
#define UDL (U | D | L)
#define ULR (U | L | R)
#define DLR (D | L | R)
#define UDLR (U | D | L | R)
}

