/* debug.c

   GNU Chess frontend

   Copyright (C) 2001-2014 Free Software Foundation, Inc.

   GNU Chess is based on the two research programs 
   Cobalt by Chua Kong-Sian and Gazebo by Stuart Cracraft.

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

   Contact Info: 
     bug-gnu-chess@gnu.org
     cracraft@ai.mit.edu, cracraft@stanfordalumni.org, cracraft@earthlink.net
*/

#include "common.h"

#ifdef DEBUG

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>

#define MAX_DEBUG 1000
#define DEBUG_FILENAME "gnuchess.debug"

/* Default to stderr */
static int debug_fd = 2;

int dbg_open(const char *name)
{
   int flags = O_WRONLY | O_CREAT | O_APPEND;
   int mode = 0777;
   
   if (name == NULL) {
      debug_fd = open(DEBUG_FILENAME, flags, mode);
   } else {
      debug_fd = open(name, flags, mode);
   }
   if (-1 == debug_fd) {
      debug_fd = 2;
      return -1;
   }
   dbg_printf("--- Opening debug log ---\n");
   return debug_fd;
}

int dbg_close(void)
{
   /* We don't want to close stderr */
   if (debug_fd == 2) {
      return -1;
   }
   dbg_printf("--- Closing debug log ---\n");
   return close(debug_fd);
}

int dbg_printf(const char *fmt, ...)
{
   va_list ap;
   char buf[MAX_DEBUG];
   struct timeval tv;
   
   gettimeofday(&tv, NULL);
#ifdef __APPLE__
   sprintf(buf, "%010ld.%06d: ", tv.tv_sec, tv.tv_usec);
#else
   sprintf(buf, "%010ld.%06ld: ", tv.tv_sec, tv.tv_usec);
#endif
   write(debug_fd, buf, strlen(buf));

   va_start(ap, fmt);
   vsnprintf(buf, MAX_DEBUG, fmt, ap);
   va_end(ap);

   return write(debug_fd, buf, strlen(buf));
}

#else /* !DEBUG */

int dbg_open(const char *name __attribute__((unused)) ) { return 0; }
int dbg_printf(const char *fmt __attribute__((unused)), ...) { return 0; }
int dbg_close(void) { return 0; }

#endif /* DEBUG */
