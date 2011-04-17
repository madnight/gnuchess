/* piece.cpp

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


// piece.cpp

// includes

#include <cstring>

#include "colour.h"
#include "piece.h"
#include "util.h"

namespace adapter {
  
// "constants"

static const uint8 MakePawn[ColourNb] = { PieceNone256, BlackPawn256, WhitePawn256 }; // -BW

static const uint8 PieceFrom12[12] = {
   BlackPawn256,   WhitePawn256,
   BlackKnight256, WhiteKnight256,
   BlackBishop256, WhiteBishop256,
   BlackRook256,   WhiteRook256,
   BlackQueen256,  WhiteQueen256,
   BlackKing256,   WhiteKing256,
};

static const char PieceString[12+1] = "pPnNbBrRqQkK";

// variables

static sint8 PieceTo12[256];

// functions

// piece_init()

void piece_init() {

   int piece;

   for (piece = 0; piece < 256; piece++) PieceTo12[piece] = -1;

   for (piece = 0; piece < 12; piece++) {
      PieceTo12[PieceFrom12[piece]] = piece;
   }
}

// piece_is_ok()

bool piece_is_ok(int piece) {

   if (piece < 0 || piece >= 256) return false;

   if (PieceTo12[piece] < 0) return false;

   return true;
}

// piece_make_pawn()

int piece_make_pawn(int colour) {

   ASSERT(colour_is_ok(colour));

   return MakePawn[colour];
}

// piece_pawn_opp()

int piece_pawn_opp(int piece) {

   ASSERT(piece==BlackPawn256||piece==WhitePawn256);

   return piece ^ 15;
}

// piece_colour()

int piece_colour(int piece) {

   ASSERT(piece_is_ok(piece));

   return piece & 3;
}

// piece_type()

int piece_type(int piece) {

   ASSERT(piece_is_ok(piece));

   return piece & ~3;
}

// piece_is_pawn()

bool piece_is_pawn(int piece) {

   ASSERT(piece_is_ok(piece));

   return (piece & PawnFlags) != 0;
}

// piece_is_knight()

bool piece_is_knight(int piece) {

   ASSERT(piece_is_ok(piece));

   return (piece & KnightFlag) != 0;
}

// piece_is_bishop()

bool piece_is_bishop(int piece) {

   ASSERT(piece_is_ok(piece));

   return (piece & QueenFlags) == BishopFlag;
}

// piece_is_rook()

bool piece_is_rook(int piece) {

   ASSERT(piece_is_ok(piece));

   return (piece & QueenFlags) == RookFlag;
}

// piece_is_queen()

bool piece_is_queen(int piece) {

   ASSERT(piece_is_ok(piece));

   return (piece & QueenFlags) == QueenFlags;
}

// piece_is_king()

bool piece_is_king(int piece) {

   ASSERT(piece_is_ok(piece));

   return (piece & KingFlag) != 0;
}

// piece_is_slider()

bool piece_is_slider(int piece) {

   ASSERT(piece_is_ok(piece));

   return (piece & QueenFlags) != 0;
}

// piece_to_12()

int piece_to_12(int piece) {

   ASSERT(piece_is_ok(piece));

   return PieceTo12[piece];
}

// piece_from_12()

int piece_from_12(int piece) {

   ASSERT(piece>=0&&piece<12);

   return PieceFrom12[piece];
}

// piece_to_char()

int piece_to_char(int piece) {

   ASSERT(piece_is_ok(piece));

   return PieceString[piece_to_12(piece)];
}

// piece_from_char()

int piece_from_char(int c) {

   const char * ptr;

   ptr = strchr(PieceString,c);
   if (ptr == NULL) return PieceNone256;

   return piece_from_12(ptr-PieceString);
}

// char_is_piece()

bool char_is_piece(int c) {

   return strchr("PNBRQK",c) != NULL;
}

}  // namespace adapter

// end of piece.cpp

