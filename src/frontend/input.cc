/* GNU Chess 5.0 - input.c - Input thread and related
   Copyright (c) 2002 Free Software Foundation, Inc.

   GNU Chess is based on the two research programs 
   Cobalt by Chua Kong-Sian and Gazebo by Stuart Cracraft.

   GNU Chess is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GNU Chess is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU Chess; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Contact Info: 
     bug-gnu-chess@gnu.org
     cracraft@ai.mit.edu, cracraft@stanfordalumni.org, cracraft@earthlink.net
     lukas@debian.org
*/

/*
 * All the pthread stuff should be hidden here, all the
 * readline things, too. (I.e., all the potentially troublesome
 * libraries.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>

#include "common.h"
#include "components.h"
#include "gettext.h"

#define _(str) gettext (str)

#ifdef HAVE_LIBREADLINE
# ifdef HAVE_READLINE_READLINE_H
#  include <readline/readline.h>
#  include <readline/history.h>
# else
extern char* readline(char *);
extern void add_history(char *);
# endif
#endif
void (*get_line)(char * p);

/* Variable used to communicate with the main thread */
volatile int input_status = INPUT_NONE;
  
char userinputstr[MAXSTR];

int wait_for_input = 1;
pthread_cond_t      input_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     input_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * The readline get_line(), notice that even if we have readline,
 * it is not used if stdin is not a tty.
 */

#ifdef HAVE_LIBREADLINE
void getline_readline(char * p)
{
  char *inp;

  inp = readline(p);
  if (inp && *inp) {
    strncpy(userinputstr, inp, MAXSTR-1);
    userinputstr[MAXSTR-1] = '\0';
    add_history(userinputstr);
  } else {
    userinputstr[0] = '\0';
  }
  if (inp) {
    free(inp);
  }
}
#endif /* HAVE_LIBREADLINE */

/* The generic input routine */

void getline_standard(char *p)
{
  if (!(flags & XBOARD)) {
    fputs(p, stdout);
    fflush(stdout);
  }
  fgets(userinputstr, MAXSTR, stdin);
}

/*
 * Sends a char string message from the input to the frontend.
 * The message must be a command or a move.
 */
int SendToFrontend( char msg[] )
{
    int outError=0;
    int msg_size = strlen( msg );
    int msg_count=0;

    msg_count = write( pipefd_i2f[1], msg, msg_size );

    if (msg_count == -1)
      outError = errno;
    else
      outError = 0;

    if ( ( msg_count == msg_size ) && ( outError == 0 ) ) {
        return 1;
    } else {
        printf( "Error sending message to frontend.\n" );
        assert( 0 );
        return 0;
    }
}

void input_wakeup( void )
{
  pthread_mutex_lock( &input_mutex );
  wait_for_input = 1;
  pthread_cond_signal( &input_cond );
  pthread_mutex_unlock( &input_mutex );
}

void *input_func(void *arg __attribute__((unused)) )
{
  char prompt[MAXSTR] = "";

  while (!(flags & QUIT)) {
    if (!(flags & XBOARD)) {

      pthread_mutex_lock( &input_mutex );
      while ( !wait_for_input )
        pthread_cond_wait( &input_cond, &input_mutex );
      pthread_mutex_unlock( &input_mutex );

      sprintf(prompt,"%s (%d) : ", 
	      RealSide ? _("Black") : _("White"), 
	      (RealGameCnt+1)/2 + 1 );
    } else {
      sprintf(prompt,"");
    }
    get_line(prompt);
    SendToFrontend( userinputstr );
#ifdef HAVE_LIBREADLINE
    SendToFrontend( "\n" );
#endif
    pthread_mutex_lock( &input_mutex );
    wait_for_input = 0;
    pthread_mutex_unlock( &input_mutex );
    /* TODO Improve this test */
    if ( strncmp(userinputstr,"quit",4) == 0 ) {
        break;
    }
  }
  return NULL;
}

void InitInput(void)
{
#ifdef HAVE_LIBREADLINE
  if (isatty(STDIN_FILENO)) {
    rl_catch_signals = 0; /* Prevent readline from catching signals, e.g. SIGINT. */
    get_line = getline_readline;
    using_history();
  } else {
    get_line = getline_standard;
  }
#else
  get_line = getline_standard;
#endif
}
