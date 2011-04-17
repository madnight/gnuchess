/* protocol.h

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


// protocol.h

#ifndef PROTOCOL_H
#define PROTOCOL_H

// includes

#include "util.h"

namespace engine {

// functions

extern void loop  ();
extern void event ();

extern void get   (char string[], int size);
extern void send  (const char format[], ...);

}  // namespace engine

#endif // !defined PROTOCOL_H

// end of protocol.h

