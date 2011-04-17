/* GNU Chess 5.90 - components.cc - Pipes shared across modules

   Copyright (c) 2001-2011 Free Software Foundation, Inc.

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

#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

#include "components.h"

/* Main loop of adapter */
namespace adapter {
  int main_adapter(int argc,char *argv[]);
}

/* Main loop of engine */
namespace engine {
  int main_engine(int argc,char *argv[]);
}

/* Adapter thread */
pthread_t adapter_thread;

/* Engine thread */
pthread_t engine_thread;

/* Pipes to communicate frontend and adapter */
int pipefd_f2a[2];
int pipefd_a2f[2];

/* Pipes to communicate adapter and engine */
int pipefd_a2e[2];
int pipefd_e2a[2];

/*
 * Entry point for the adapter thread
 */
void *adapter_func(void *arg)
{
  /* Start engine */
  InitEngine();
  /* Start adapter main loop */
  adapter::main_adapter( 0, 0 );
}

/*
 * Starts the adapter (based on Polyglot 1.4) on a separate thread.
 */
void InitAdapter()
{
  /* Create pipes to communicate frontend and adapter. */
  if ( ( pipe( pipefd_f2a ) != 0 ) || ( pipe( pipefd_a2f ) != 0 ) ) {
    printf( "Error while creating pipes.\n" );
    exit( 1 );
  }

  /* Start adapter thread */
  pthread_create(&adapter_thread, NULL, adapter_func, NULL);
}

/*
 * Entry point for the engine thread
 */
void *engine_func(void *arg)
{
  /* Start engine main loop */
  engine::main_engine( 0, 0 );
}

/*
 * Starts the engine (based on Fruit 2.1) on a separate thread.
 */

void InitEngine()
{
  /* Create pipes to communicate adapter and engine. */
  if ( ( pipe( pipefd_a2e ) != 0 ) || ( pipe( pipefd_e2a ) != 0 ) ) {
    printf( "Error while creating pipes.\n" );
    exit( 1 );
  }

  /* Start engine thread */
  pthread_create(&engine_thread, NULL, engine_func, NULL);
}
