/* san.h

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


// san.h

#ifndef SAN_H
#define SAN_H

// includes

#include "board.h"
#include "util.h"

namespace adapter {

// functions

extern bool move_to_san         (int move, const board_t * board, char string[], int size);
extern int  move_from_san       (const char string[], const board_t * board);

extern int  move_from_san_debug (const char string[], const board_t * board);

}  // namespace adapter

#endif // !defined SAN_H

// end of san.h

