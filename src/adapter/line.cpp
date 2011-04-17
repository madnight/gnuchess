/* line.cpp

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


// line.cpp

// includes

#include <cstring>

#include "board.h"
#include "line.h"
#include "move.h"
#include "move_do.h"
#include "move_legal.h"
#include "san.h"
#include "util.h"

namespace adapter {
  
// constants

static const bool Strict = false; // false
static const bool UseDebug = false; // false

static const int StringSize = 1024;

// functions

// line_is_ok()

bool line_is_ok(const move_t line[]) {

   int move;

   if (line == NULL) return false;

   while ((move = *line++) != MoveNone) {
      if (!move_is_ok(move)) return false;
   }

   return true;
}

// line_clear()

void line_clear(move_t line[]) {

   ASSERT(line!=NULL);

   *line = MoveNone;
}

// line_copy()

void line_copy(move_t dst[], const move_t src[]) {

   ASSERT(dst!=NULL);
   ASSERT(src!=NULL);

   ASSERT(dst!=src);

   while ((*dst++ = *src++) != MoveNone)
      ;
}

// line_from_can()

bool line_from_can (move_t line[], const board_t * board, const char string[], int size) {

   int pos;
   char new_string[StringSize], *p;
   int move;
   board_t new_board[1];

   ASSERT(line!=NULL);
   ASSERT(board_is_ok(board));
   ASSERT(string!=NULL);
   ASSERT(size>=LineSize);

   // init

   pos = 0;
   board_copy(new_board,board);

   // loop

   strcpy(new_string,string); // HACK

   for (p = strtok(new_string," "); p != NULL; p = strtok(NULL," ")) {

      move = move_from_can(p,new_board);

      ASSERT(move!=MoveNone);
      ASSERT(move_is_legal(move,new_board));

      if (move == MoveNone || !move_is_legal(move,new_board)) break; // HACK: ignore illegal moves

      if (pos >= size) return false;
      line[pos++] = move;

      move_do(new_board,move);
   }

   if (pos >= size) return false;
   line[pos] = MoveNone;

   return true;
}

// line_to_can()

bool line_to_can(const move_t line[], const board_t * board, char string[], int size) {

   board_t new_board[1];
   int pos;
   int move;

   ASSERT(line_is_ok(line));
   ASSERT(board_is_ok(board));
   ASSERT(string!=NULL);
   ASSERT(size>=StringSize);

   // init

   if (size < StringSize) return false;

   board_copy(new_board,board);
   pos = 0;

   // loop

   while ((move = *line++) != MoveNone) {

      if (pos != 0) {
         if (pos >= size) return false;
         string[pos++] = ' ';
      }

      if (!move_to_can(move,new_board,&string[pos],size-pos)) return false;
      pos += strlen(&string[pos]);

      move_do(new_board,move);
   }

   if (pos >= size) return false;
   string[pos] = '\0';

   return true;
}

// line_to_san()

bool line_to_san(const move_t line[], const board_t * board, char string[], int size) {

   board_t new_board[1];
   int pos;
   int move;
   char move_string[256];

   ASSERT(line_is_ok(line));
   ASSERT(board_is_ok(board));
   ASSERT(string!=NULL);
   ASSERT(size>=StringSize);

   // init

   if (size < StringSize) return false;

   board_copy(new_board,board);
   pos = 0;

   // loop

   while ((move = *line++) != MoveNone) {

      if (pos != 0) {
         if (pos >= size) return false;
         string[pos++] = ' ';
      }

      if (!move_is_legal(move,new_board)
       || !move_to_san(move,new_board,&string[pos],size-pos)) {

         if (Strict || UseDebug) {

            move_to_can(move,new_board,move_string,256);
            my_log("POLYGLOT ILLEGAL MOVE IN LINE %s\n",move_string);

            board_disp(new_board);
         }

         if (Strict) my_fatal("line_to_san(): illegal move\n");

         break;
      }

      pos += strlen(&string[pos]);

      move_do(new_board,move);
   }

   if (pos >= size) return false;
   string[pos] = '\0';

   return true;
}

}  // namespace adapter

// end of line.cpp

