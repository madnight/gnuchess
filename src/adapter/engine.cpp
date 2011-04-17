/* engine.cpp

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


// engine.cpp

// includes

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <unistd.h>

#include "engine.h"
#include "io.h"
#include "option.h"
#include "util.h"
#include "components.h"

namespace adapter {
  
// constants

static const bool UseNice = false;

static const int StringSize = 4096;

// variables

engine_t Engine[1];

// prototypes

// functions

// engine_is_ok()

bool engine_is_ok(const engine_t * engine) {

   if (engine == NULL) return false;

   if (!io_is_ok(engine->io)) return false;

   return true;
}

// engine_open()

void engine_open(engine_t * engine) {

   engine->io->in_fd = pipefd_e2a[0];
   engine->io->out_fd = pipefd_a2e[1];
   engine->io->name = "ENGINE";

   io_init(engine->io);
}

// engine_close()

void engine_close(engine_t * engine) {

   ASSERT(engine_is_ok(engine));

   io_close(engine->io);
}

// engine_get()

void engine_get(engine_t * engine, char string[], int size) {

   ASSERT(engine_is_ok(engine));
   ASSERT(string!=NULL);
   ASSERT(size>=256);

   while (!io_line_ready(engine->io)) {
      io_get_update(engine->io);
   }

   if (!io_get_line(engine->io,string,size)) { // EOF
      exit(EXIT_SUCCESS);
   }
}

// engine_send()

void engine_send(engine_t * engine, const char format[], ...) {

   va_list arg_list;
   char string[StringSize];

   ASSERT(engine_is_ok(engine));
   ASSERT(format!=NULL);

   // format

   va_start(arg_list,format);
   vsprintf(string,format,arg_list);
   va_end(arg_list);

   // send

   io_send(engine->io,"%s",string);
}

// engine_send_queue()

void engine_send_queue(engine_t * engine, const char format[], ...) {

   va_list arg_list;
   char string[StringSize];

   ASSERT(engine_is_ok(engine));
   ASSERT(format!=NULL);

   // format

   va_start(arg_list,format);
   vsprintf(string,format,arg_list);
   va_end(arg_list);

   // send

   io_send_queue(engine->io,"%s",string);
}

}  // namespace adapter

// end of engine.cpp

