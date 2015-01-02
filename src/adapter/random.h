/* random.h

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


// random.h

#ifndef RANDOM_H
#define RANDOM_H

// includes

#include "util.h"

namespace adapter {

// constants

const int RandomNb = 781;

// macros

#define RANDOM_64(n) (Random64[(n)])

// "constants"

extern const uint64 Random64[RandomNb];

// functions

extern void   random_init ();
extern uint64 random_64   (int n);

}  // namespace adapter

#endif // !defined RANDOM_H

// end of random.h

