/* colour.h

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


// colour.h

#ifndef COLOUR_H
#define COLOUR_H

// includes

#include "util.h"

namespace adapter {

// constants

const int BlackFlag = 1 << 0;
const int WhiteFlag = 1 << 1;

const int ColourNone = 0;
const int Black      = BlackFlag;
const int White      = WhiteFlag;
const int ColourNb   = 3;

// functions

extern bool colour_is_ok    (int colour);

extern bool colour_is_white (int colour);
extern bool colour_is_black (int colour);
extern bool colour_equal    (int colour_1, int colour_2);

extern int  colour_opp      (int colour);

}  // namespace adapter

#endif // !defined COLOUR_H

// end of colour.h

