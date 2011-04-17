/* colour.h

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


// colour.h

#ifndef COLOUR_H
#define COLOUR_H

// includes

#include "util.h"

namespace engine {

// constants

const int ColourNone = -1;
const int White = 0;
const int Black = 1;
const int ColourNb = 2;

const int WhiteFlag = 1 << White;
const int BlackFlag = 1 << Black;

}  // namespace engine

// macros

#define COLOUR_IS_OK(colour)    (((colour)&~1)==0)

#define COLOUR_IS_WHITE(colour) ((colour)==White)
#define COLOUR_IS_BLACK(colour) ((colour)!=White)

#define COLOUR_IS(piece,colour) (FLAG_IS((piece),COLOUR_FLAG(colour)))
#define FLAG_IS(piece,flag)     (((piece)&(flag))!=0)

#define COLOUR_OPP(colour)      ((colour)^(White^Black))
#define COLOUR_FLAG(colour)     ((colour)+1)

#endif // !defined COLOUR_H

// end of colour.h

