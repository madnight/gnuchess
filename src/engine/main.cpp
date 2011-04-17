/* main.cpp

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


// main.cpp

// includes

#include <cstdio>
#include <cstdlib>

#include "attack.h"
#include "book.h"
#include "hash.h"
#include "move_do.h"
#include "option.h"
#include "pawn.h"
#include "piece.h"
#include "protocol.h"
#include "random.h"
#include "square.h"
#include "trans.h"
#include "util.h"
#include "value.h"
#include "vector.h"
#include "components.h"

namespace engine {

// Streams used to communicate with the adapter

FILE *pipefd_a2e_0_stream;
FILE *pipefd_e2a_1_stream;

// functions

// main()

int main_engine(int argc, char * argv[]) {

   // Get reference stream of pipes to communicate with the adapter

   pipefd_a2e_0_stream = fdopen( pipefd_a2e[0], "r" );
   pipefd_e2a_1_stream = fdopen( pipefd_e2a[1], "w" );
   setvbuf(pipefd_a2e_0_stream,NULL,_IONBF,0);
   setvbuf(pipefd_e2a_1_stream,NULL,_IONBF,0); // _IOLBF breaks on Windows!

   // init

   my_random_init(); // for opening book

   // early initialisation (the rest is done after UCI options are parsed in protocol.cpp)

   option_init();

   square_init();
   piece_init();
   pawn_init_bit();
   value_init();
   vector_init();
   attack_init();
   move_do_init();

   random_init();
   hash_init();

   trans_init(Trans);
   book_init();

   // loop

   loop();

   return EXIT_SUCCESS;
}

}  // namespace engine

// end of main.cpp

