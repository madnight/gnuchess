/* main.cpp

   GNU Chess protocol adapter

   Copyright (C) 2001-2015 Free Software Foundation, Inc.

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


// main.cpp

// includes

#include <pthread.h>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "adapter.h"
#include "attack.h"
#include "board.h"
#include "book.h"
#include "book_make.h"
#include "book_merge.h"
#include "engine.h"
#include "epd.h"
#include "fen.h"
#include "hash.h"
#include "list.h"
#include "main.h"
#include "move.h"
#include "move_gen.h"
#include "option.h"
#include "piece.h"
#include "search.h"
#include "square.h"
#include "uci.h"
#include "util.h"

namespace adapter {

// constants

static const char * const Version = "1.4";

static const bool BlockSignal = false; // true on Windows

static const int SearchDepth = 63;
static const double SearchTime = 3600.0;

static const int StringSize = 4096;

// variables

static bool Init;
extern unsigned int HashSize;
pthread_mutex_t adapter_init_mutex;
pthread_cond_t adapter_init_cond;

// prototypes

static void parse_option ();
static bool parse_line   (char line[], char * * name_ptr, char * * value_ptr);

static void stop_search  ();

// functions

// main()

int main_adapter(int argc, char * argv[]) {

   // init

   Init = false;

   if (BlockSignal) {
      signal(SIGINT,SIG_IGN);
      signal(SIGTERM,SIG_IGN);
      signal(SIGPIPE,SIG_IGN);
   }

   util_init();

   option_init();

   square_init();
   piece_init();
   attack_init();

   hash_init();

   my_random_init();

   // build book

   if (argc >= 2 && my_string_equal(argv[1],"make-book")) {
      book_make(argc,argv);
      return EXIT_SUCCESS;
   }

   if (argc >= 2 && my_string_equal(argv[1],"merge-book")) {
      book_merge(argc,argv);
      return EXIT_SUCCESS;
   }

   // read options

   if (argc == 2) option_set("OptionFile",argv[1]); // HACK for compatibility

   parse_option(); // HACK: also launches the engine

   // EPD test

   if (argc >= 2 && my_string_equal(argv[1],"epd-test")) {
      epd_test(argc,argv);
      return EXIT_SUCCESS;
   }

   // opening book

   book_clear();
   if (option_get_bool("Book")) {
      int mode = BookReadOnly;
      if (option_get_bool("BookLearn")) {
         mode = BookReadWrite;
      }
      book_open(option_get_string("BookFile"),mode);
   }

   // flag adapter initialized
   pthread_mutex_lock( &adapter_init_mutex );
   pthread_cond_signal( &adapter_init_cond );
   pthread_mutex_unlock( &adapter_init_mutex );

   // adapter
   adapter_loop();

   engine_send(Engine,"quit");
   engine_close(Engine);

   return EXIT_SUCCESS;
}

// parse_option()

static void parse_option() {

   const char * file_name;
   FILE * file;
   char line[256];
   char * name, * value;

   file_name = option_get_string("OptionFile");

   file = fopen(file_name,"r");
   if (file == NULL) fprintf( stderr,"Can't open file \"%s\": %s - using defaults\n",file_name,strerror(errno));

   // PolyGlot options (assumed first)

   if ( file != NULL ) {
      while (true) {

         if (!my_file_read_line(file,line,256)) {
            my_fatal("parse_option(): missing [Engine] section\n");
         }

         if (my_string_case_equal(line,"[engine]")) break;

         if (parse_line(line,&name,&value)) option_set(name,value);
      }
   }

   if (option_get_bool("Log")) {
      my_log_open(option_get_string("LogFile"));
   }

   my_log("POLYGLOT *** START ***\n");
   my_log("POLYGLOT INI file \"%s\"\n",file_name);

   // engine options (assumed second and last)

   engine_open(Engine);
   Init = true; // engine has been launched
   uci_open(Uci,Engine);

   if ( file != NULL ) {
      while (my_file_read_line(file,line,256)) {

         if (line[0] == '[') my_fatal("parse_option(): unknown section %s\n",line);

         if (parse_line(line,&name,&value)) {
            if ( strcmp(name,"Hash") == 0 ) {
               sscanf( value, "%d", &HashSize );
            }
            uci_send_option(Uci,name,"%s",value);
         }
      }
   }

   uci_send_isready(Uci);

   if ( file != NULL ) {
      fclose(file);
   }

   if (my_string_equal(option_get_string("EngineName"),"<empty>")) {
      option_set("EngineName",Uci->name);
   }
}

// parse_line()

static bool parse_line(char line[], char * * name_ptr, char * * value_ptr) {

   char * ptr;
   char * name, * value;

   ASSERT(line!=NULL);
   ASSERT(name_ptr!=NULL);
   ASSERT(value_ptr!=NULL);

   // remove comments

   ptr = strchr(line,';');
   if (ptr != NULL) *ptr = '\0';

   ptr = strchr(line,'#');
   if (ptr != NULL) *ptr = '\0';

   // split at '='

   ptr = strchr(line,'=');
   if (ptr == NULL) return false;

   name = line;
   value = ptr+1;

   // cleanup name

   while (*name == ' ') name++; // remove leading spaces

   while (ptr > name && ptr[-1] == ' ') ptr--; // remove trailing spaces
   *ptr = '\0';

   if (*name == '\0') return false;

   // cleanup value

   ptr = &value[strlen(value)]; // pointer to string terminator

   while (*value == ' ') value++; // remove leading spaces

   while (ptr > value && ptr[-1] == ' ') ptr--; // remove trailing spaces
   *ptr = '\0';

   if (*value == '\0') return false;

   // end

   *name_ptr = name;
   *value_ptr = value;

   return true;
}

// quit()

void quit() {

   char string[StringSize];

   my_log("POLYGLOT *** QUIT ***\n");

   if (Init) {

      stop_search();
      engine_send(Engine,"quit");

      pthread_exit(NULL);

      // wait for the engine to quit

      while (true) {
         engine_get(Engine,string,StringSize); // HACK: calls exit() on receiving EOF
      }

      uci_close(Uci);
   }

   //exit(EXIT_SUCCESS);
}

// stop_search()

static void stop_search() {

   if (Init && Uci->searching) {

      ASSERT(Uci->searching);
      ASSERT(Uci->pending_nb>=1);

      my_log("POLYGLOT STOP SEARCH\n");

/*
      engine_send(Engine,"stop");
      Uci->searching = false;
*/

      if (option_get_bool("SyncStop")) {
         uci_send_stop_sync(Uci);
      } else {
         uci_send_stop(Uci);
      }
   }
}

}  // namespace adapter

// end of main.cpp
