/* engine.h

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


// engine.h

#ifndef ENGINE_H
#define ENGINE_H

// includes

#include "io.h"
#include "util.h"

namespace adapter {
  
// types

struct engine_t {
   io_t io[1];
};

// variables

extern engine_t Engine[1];

// functions

extern bool engine_is_ok      (const engine_t * engine);

extern void engine_open       (engine_t * engine);
extern void engine_close      (engine_t * engine);

extern void engine_get        (engine_t * engine, char string[], int size);

extern void engine_send       (engine_t * engine, const char format[], ...);
extern void engine_send_queue (engine_t * engine, const char format[], ...);

}  // namespace adapter

#endif // !defined ENGINE_H

// end of engine.h

