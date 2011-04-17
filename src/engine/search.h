/* search.h

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


// search.h

#ifndef SEARCH_H
#define SEARCH_H

// includes

#include <csetjmp>

#include "board.h"
#include "list.h"
#include "move.h"
#include "util.h"

namespace engine {

// constants

const int DepthMax = 64;
const int HeightMax = 256;

const int SearchNormal = 0;
const int SearchShort  = 1;

const int SearchUnknown = 0;
const int SearchUpper   = 1;
const int SearchLower   = 2;
const int SearchExact   = 3;

// types

struct search_input_t {
   board_t board[1];
   list_t list[1];
   bool infinite;
   bool depth_is_limited;
   int depth_limit;
   bool time_is_limited;
   double time_limit_1;
   double time_limit_2;
};

struct search_info_t {
   jmp_buf buf;
   bool can_stop;
   bool stop;
   int check_nb;
   int check_inc;
   double last_time;
};

struct search_root_t {
   list_t list[1];
   int depth;
   int move;
   int move_pos;
   int move_nb;
   int last_value;
   bool bad_1;
   bool bad_2;
   bool change;
   bool easy;
   bool flag;
};

struct search_best_t {
   int move;
   int value;
   int flags;
   int depth;
   mv_t pv[HeightMax];
};

struct search_current_t {
   board_t board[1];
   my_timer_t timer[1];
   int max_depth;
   sint64 node_nb;
   double time;
   double speed;
   double cpu;
};

// variables

extern search_input_t SearchInput[1];
extern search_info_t SearchInfo[1];
extern search_best_t SearchBest[1];
extern search_root_t SearchRoot[1];
extern search_current_t SearchCurrent[1];

// functions

extern bool depth_is_ok           (int depth);
extern bool height_is_ok          (int height);

extern void search_clear          ();
extern void search                ();

extern void search_update_best    ();
extern void search_update_root    ();
extern void search_update_current ();

extern void search_check          ();

}  // namespace engine

#endif // !defined SEARCH_H

// end of search.h

