/* io.h

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


// io.h

#ifndef IO_H
#define IO_H

// includes

#include "util.h"

namespace adapter {
  
// constants

const int BufferSize = 16384;

// types

struct io_t {

   int in_fd;
   int out_fd;

   const char * name;

   bool in_eof;

   sint32 in_size;
   sint32 out_size;

   char in_buffer[BufferSize];
   char out_buffer[BufferSize];
};

// functions

extern bool io_is_ok      (const io_t * io);

extern void io_init       (io_t * io);
extern void io_close      (io_t * io);

extern void io_get_update (io_t * io);

extern bool io_line_ready (const io_t * io);
extern bool io_get_line   (io_t * io, char string[], int size);

extern void io_send       (io_t * io, const char format[], ...);
extern void io_send_queue (io_t * io, const char format[], ...);

}  // namespace adapter

#endif // !defined IO_H

// end of io.h

