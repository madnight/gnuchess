/* line.h

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


// line.h

#ifndef LINE_H
#define LINE_H

// includes

#include "board.h"
#include "move.h"
#include "util.h"

namespace adapter {
  
// constants

const int LineSize = 256;

// functions

extern bool line_is_ok    (const move_t line[]);

extern void line_clear    (move_t line[]);
extern void line_copy     (move_t dst[], const move_t src[]);

extern bool line_from_can (move_t line[], const board_t * board, const char string[], int size);

extern bool line_to_can   (const move_t line[], const board_t * board, char string[], int size);
extern bool line_to_san   (const move_t line[], const board_t * board, char string[], int size);

}  // namespace adapter

#endif // !defined LINE_H

// end of line.h

