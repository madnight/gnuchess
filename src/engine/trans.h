/* trans.h

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


// trans.h

#ifndef TRANS_H
#define TRANS_H

// includes

#include "util.h"

namespace engine {

// types

typedef struct trans trans_t;

// variables

extern trans_t Trans[1];

// functions

extern bool trans_is_ok    (const trans_t * trans);

extern void trans_init     (trans_t * trans);
extern void trans_alloc    (trans_t * trans);
extern void trans_free     (trans_t * trans);

extern void trans_clear    (trans_t * trans);
extern void trans_inc_date (trans_t * trans);

extern void trans_store    (trans_t * trans, uint64 key, int move, int depth, int min_value, int max_value);
extern bool trans_retrieve (trans_t * trans, uint64 key, int * move, int * min_depth, int * max_depth, int * min_value, int * max_value);

extern void trans_stats    (const trans_t * trans);

}  // namespace engine

#endif // !defined TRANS_H

// end of trans.h

