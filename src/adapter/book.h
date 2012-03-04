/* book.h

   GNU Chess protocol adapter

   Copyright (C) 2001-2012 Free Software Foundation, Inc.

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

namespace adapter {
  
// constants

const int BookReadOnly   = 1;
const int BookReadWrite  = 2;

// functions

extern void book_clear      ();

extern void book_open       (const char file_name[], int mode);
extern void book_close      ();

extern bool is_in_book      (const board_t * board);
extern int  book_move       (const board_t * board, bool random);
extern int  book_move       (const board_t * board, bool random, bool worst);
extern void book_disp       (const board_t * board);

extern void book_learn_move (const board_t * board, int move, int result);
extern void book_flush      ();

}  // namespace adapter

#endif // !defined BOOK_H

// end of book.h

