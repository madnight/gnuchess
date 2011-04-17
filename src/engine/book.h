/* book.h

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


// book.h

#ifndef BOOK_H
#define BOOK_H

// includes

#include "board.h"
#include "util.h"

namespace engine {

// functions

extern void book_init  ();

extern void book_open  (const char file_name[]);
extern void book_close ();

extern int  book_move  (board_t * board);

}  // namespace engine

#endif // !defined BOOK_H

// end of book.h

