/* pgn.h

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


// pgn.h

#ifndef PGN_H
#define PGN_H

// includes

#include <cstdio>

#include "util.h"

namespace adapter {
  
// constants

const int PGN_STRING_SIZE = 256;

// types

struct pgn_t {

   FILE * file;

   int char_hack;
   int char_line;
   int char_column;
   bool char_unread;
   bool char_first;

   int token_type;
   char token_string[PGN_STRING_SIZE];
   int token_length;
   int token_line;
   int token_column;
   bool token_unread;
   bool token_first;

   char result[PGN_STRING_SIZE];
   char fen[PGN_STRING_SIZE];

   int move_line;
   int move_column;
};

// functions

extern void pgn_open      (pgn_t * pgn, const char file_name[]);
extern void pgn_close     (pgn_t * pgn);

extern bool pgn_next_game (pgn_t * pgn);
extern bool pgn_next_move (pgn_t * pgn, char string[], int size);

}  // namespace adapter

#endif // !defined PGN_H

// end of pgn.h

