/* attack.h

   GNU Chess protocol adapter

   Copyright (C) 2001-2011 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


// attack.h

#ifndef ATTACK_H
#define ATTACK_H

// includes

#include "board.h"
#include "util.h"

namespace adapter {
  
// constants

const int IncNone = 0;

// "constants"

extern const sint8 KnightInc[8+1];
extern const sint8 BishopInc[4+1];
extern const sint8 RookInc[4+1];
extern const sint8 QueenInc[8+1];
extern const sint8 KingInc[8+1];

// functions

extern void attack_init  ();

extern bool is_in_check  (const board_t * board, int colour);
extern bool is_attacked  (const board_t * board, int to, int colour);
extern bool piece_attack (const board_t * board, int piece, int from, int to);

extern bool is_pinned    (const board_t * board, int from, int to, int colour);

}  // namespace adapter

#endif // !defined ATTACK_H

// end of attack.h

