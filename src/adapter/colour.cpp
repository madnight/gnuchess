/* colour.cpp

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


// colour.cpp

// includes

#include "colour.h"
#include "util.h"

namespace adapter {

// functions

// colour_is_ok()

bool colour_is_ok(int colour) {

   return colour == Black || colour == White;
}

// colour_is_white()

bool colour_is_white(int colour) {

   ASSERT(colour_is_ok(colour));

   return colour == White;
}

// colour_is_black()

bool colour_is_black(int colour) {

   ASSERT(colour_is_ok(colour));

   return colour == Black;
}

// colour_equal()

bool colour_equal(int colour_1, int colour_2) {

   ASSERT(colour_is_ok(colour_2));

   return (colour_1 & colour_2) != 0;
}

// colour_opp()

int colour_opp(int colour) {

   ASSERT(colour_is_ok(colour));

   return colour ^ (BlackFlag^WhiteFlag);
}

}  // namespace adapter

// end of colour.cpp

