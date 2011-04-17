/* search.h

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


// search.h

#ifndef SEARCH_H
#define SEARCH_H

// includes

#include "board.h"
#include "util.h"

namespace adapter {
  
// constants

const int DepthMax = 63;

// functions

extern void search       (const board_t * board, int depth_max, double time_max);
extern void search_perft (const board_t * board, int depth_max);

}  // namespace adapter

#endif // !defined SEARCH_H

// end of search.h

