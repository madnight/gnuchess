/* move.h

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


// move.h

#ifndef MOVE_H
#define MOVE_H

// includes

#include "board.h"
#include "util.h"

namespace adapter {

// constants

const int MoveNone = 0; // HACK: a1a1 cannot be a legal move

const int MovePromoteKnight = 1 << 12;
const int MovePromoteBishop = 2 << 12;
const int MovePromoteRook   = 3 << 12;
const int MovePromoteQueen  = 4 << 12;
const int MoveFlags         = 7 << 12;

// types

typedef uint16 move_t;

// functions

extern bool move_is_ok          (int move);

extern int  move_make           (int from, int to);
extern int  move_make_flags     (int from, int to, int flags);

extern int  move_from           (int move);
extern int  move_to             (int move);
extern int  move_promote_hack   (int move);

extern bool move_is_capture     (int move, const board_t * board);
extern bool move_is_promote     (int move);
extern bool move_is_en_passant  (int move, const board_t * board);
extern bool move_is_castle      (int move, const board_t * board);

extern int  move_piece          (int move, const board_t * board);
extern int  move_capture        (int move, const board_t * board);
extern int  move_promote        (int move, const board_t * board);

extern bool move_is_check       (int move, const board_t * board);
extern bool move_is_mate        (int move, const board_t * board);

extern int  move_order          (int move);

extern bool move_to_can         (int move, const board_t * board, char string[], int size);
extern int  move_from_can       (const char string[], const board_t * board);

extern void move_disp           (int move, const board_t * board);

}  // namespace adapter

#endif // !defined MOVE_H

// end of move.h

