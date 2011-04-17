/* move_check.h

   GNU Chess engine

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


// move_check.h

#ifndef MOVE_CHECK_H
#define MOVE_CHECK_H

// includes

#include "board.h"
#include "list.h"
#include "util.h"

namespace engine {

// functions

extern void gen_quiet_checks (list_t * list, board_t * board);

extern bool move_is_check    (int move, board_t * board);

}  // namespace engine

#endif // !defined MOVE_CHECK_H

// end of move_check.h

