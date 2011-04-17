/* move_evasion.h

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


// move_evasion.h

#ifndef MOVE_EVASION_H
#define MOVE_EVASION_H

// includes

#include "attack.h"
#include "board.h"
#include "list.h"
#include "util.h"

namespace engine {

// functions

extern void gen_legal_evasions  (list_t * list, const board_t * board, const attack_t * attack);
extern void gen_pseudo_evasions (list_t * list, const board_t * board, const attack_t * attack);

extern bool legal_evasion_exist (const board_t * board, const attack_t * attack);

}  // namespace engine

#endif // !defined MOVE_EVASION_H

// end of move_evasion.h

