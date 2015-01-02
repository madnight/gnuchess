/* fen.h

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


// fen.h

#ifndef FEN_H
#define FEN_H

// includes

#include "board.h"
#include "util.h"

namespace adapter {

// "constants"

extern const char * StartFen;

// functions

extern bool board_from_fen (board_t * board, const char string[]);
extern bool board_to_fen   (const board_t * board, char string[], int size);

}  // namespace adapter

#endif // !defined FEN_H

// end of fen.h

