/* engine.c

   GNU Chess frontend

   Copyright (C) 2001-2015 Free Software Foundation, Inc.

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include "common.h"
#include "components.h"
#include "gettext.h"

#define _(str) gettext (str)

/* Message to be sent to the engine */
char inputstr[BUF_SIZE]="";

/* Message to be sent to the engine */
char dataToEngine[BUF_SIZE]="";

/* Whether an answer is expected from the engine or not, as a result of a command send to it. */
static int answerFromEngineExpected=false;

/* A BUF_SIZE long char string made up of null characters */
static char zerochar[BUF_SIZE];

/* Whether the prompt must be displayed or not. */
static int showprompt=1;

/* Buffer storing the input entered by the user */
char userinputbuf[BUF_SIZE]="";

/* Buffer storing the input coming from the engine */
static char engineinputbuf[BUF_SIZE]="";

/* Whether the input is a move or not (if not, it is expected to be a command). */
static int userInputIsAMove=0;

/* Flags that the color must be changed */
static int changeColor=false;

/* Flags whether a go command must be sent after a user move. */
static int autoGo=false;

/*
 * Forward declaration of functions
 */

int SendToEngine( char msg[] );
static int GetNextLine( char buf[], char line[] );
static int GetDataToEngine( char data[] );
static int AnswerFromEngineExpected( void );
static int UserInputIsAValidMove( void );

/*
 * Initializes data used in the frontend.
 */
void InitFrontend( void )
{
  /* Initialized a null char string that will be used later on. */
  int i;
  for ( i=0; i<BUF_SIZE; ++i )
    zerochar[i] = '\0';
}

/*
 * Sends a char string message from the frontend to the adapter/engine.
 * The message must be a command or a move.
 */
int SendToEngine( char msg[] )
{
    int outError=0;
    int msg_size = strlen( msg );
    int msg_count=0;

    /* The trailing '\n' is necessary. Otherwise, Polyglot will not realise of
       the new message. TODO: This should be improved. */
    msg[msg_size] = '\n';
    msg[msg_size+1] = '\0';

    msg_count = write( pipefd_f2a[1], msg, msg_size+1 );

    if (msg_count == -1)
      outError = errno;
    else
      outError = 0;

    if ( ( msg_count == msg_size+1 ) && ( outError == 0 ) ) {
        return 1;
    } else {
        printf( "Error sending message to engine.\n" );
        assert( 0 );
        return 0;
    }
}

/*
 * Reads a char string message from the adapter/engine.
 * The message must be a command or a move.
 * The message is not processed straight away, but saved in buffer 'engineinputbuf'.
 * The buffer will be read later from function nextEngineCmd();
 */
int ReadFromEngine( void )
{

  int nread=0;
  fd_set set[1];
  struct timeval time_val[1];
  int engineinputready=0;
  char engineinputaux[BUF_SIZE]="";

  /* Poll input from engine in non-blocking mode */
  FD_ZERO(set);
  FD_SET( pipefd_a2f[0],set);
  time_val->tv_sec = 0;
  time_val->tv_usec = 0;
  engineinputready = select( pipefd_a2f[0]+1, set, NULL, NULL, time_val );

  if ( engineinputready == -1 ) {
    printf( "Error reading engine input.\n" );
  } else if ( engineinputready > 0 ) {
    /* There are some data from the engine. Store it in buffer */
    strncpy( engineinputaux, zerochar, BUF_SIZE );
    nread = read( pipefd_a2f[0], engineinputaux, BUF_SIZE );
    /*write( STDOUT_FILENO, engineinputaux, BUF_SIZE );*/
    strcat( engineinputbuf, engineinputaux );
    engineinputbuf[strlen( engineinputbuf ) + nread] = '\0';
  }

  return ( engineinputready );
}

/*
 * Reads a char string entered by the user.
 * The string must be a command or a move.
 * The string is not processed straight away, but saved in buffer 'userinputbuf'.
 * The buffer will be read later from function nextUserCmd();
 */
void ReadFromUser( void )
{
  int nread=0;
  fd_set set[1];
  struct timeval time_val[1];
  int userinputready=0;
  char userinputaux[BUF_SIZE]="";

  /* Poll input from user in non-blocking mode */
  FD_ZERO(set);
  FD_SET(STDIN_FILENO,set);
  time_val->tv_sec = 0;
  time_val->tv_usec = 0;
  userinputready = select( STDIN_FILENO+1, set, NULL, NULL, time_val );

  if ( userinputready == -1 ) {
    printf( "Error reading user input.\n" );
  } else if ( userinputready > 0 ) {
    /* There are some data from the user. Store it in buffer */
    strncpy( userinputaux, zerochar, BUF_SIZE );
    nread = read( STDIN_FILENO, userinputaux, BUF_SIZE );
    strcat( userinputbuf, userinputaux );
    userinputbuf[strlen( userinputbuf ) + nread] = '\0';
  }
}

/*
 * Saves in global variable 'dataToEngine' the data to be sent to the engine.
 */
void SetDataToEngine( const char data[] )
{
  strcpy( dataToEngine, data );
}

/*
 * Writes in the (output) parameter the data that must be sent to the engine.
 *
 * Return value:
 *      1 - if there are some data to be sent
 *      0 - if there are no data to be sent
 */
static int GetDataToEngine( char data[] )
{
  if ( strcmp( dataToEngine, "" ) == 0 ) {
    strcpy( data, "" );
    return 0;
  } else {
    strcpy( data, dataToEngine );
    return 1;
  }
}

/*
 * Stores in a global flag variable whether an answer is expected from
 * the engine (1) or not (0).
 */
void ExpectAnswerFromEngine( int answerExpected )
{
  answerFromEngineExpected = answerExpected;
}

/*
 * Return value:
 *      1 - an answer is expected from the engine
 *      0 - an answer is not expected from the engine
 */
static int AnswerFromEngineExpected( void )
{
  return ( answerFromEngineExpected );
}

/*
 * If the prompt must be displayed on the standard output, according to
 * the current state, it is displayed.
 */
void ShowPrompt( void )
{
  char prompt[MAXSTR] = "";
  if ( showprompt && !(flags & XBOARD) ) {
    sprintf(prompt,"%s (%d) : ",
            RealSide ? _("Black") : _("White"),
            (RealGameCnt+1)/2 + 1 );
    fprintf( stdout, "%s", prompt );
    fflush( stdout );
    showprompt = 0;
  }
}

/*
 * Extracts a command from the user input buffer.
 *
 * The command is removed from the buffer.
 * If the command must be sent to the engine, it is sent.
 * This function relies on parse_input().
 */
void NextUserCmd( void )
{
  char userinput[BUF_SIZE]="";

  if ( strlen( userinputbuf ) > 0 ) {
    if ( flags & XBOARD ) {
      fflush( stdout );
    }
    if ( GetNextLine( userinputbuf, userinput ) > 0 ) {
      strcpy( inputstr, "" );
      strcpy( inputstr, userinput );
      dbg_printf( "USER >: %s\n", userinput );
      parse_input();
      /* Check if command/move must be sent to engine */
      if ( GetDataToEngine( dataToEngine ) ) {
        dbg_printf( "> ENGINE: %s\n", dataToEngine );
        SendToEngine( dataToEngine );
        if ( GetAutoGo() && UserInputIsAValidMove() ) {
            strcpy( userinputbuf, "go\n" );
            SetAutoGo( false );
        }
      }
      showprompt = !AnswerFromEngineExpected();
      /* Check if command was entered in manual mode */
      if ( (flags & MANUAL) && UserInputIsAValidMove() ) {
        RealGameCnt = GameCnt;
        RealSide = board.side;
        showprompt = 1;
      }
      /* Check if the color must be changed, e.g. after an undo command. */
      if ( changeColor ) {
        RealGameCnt = GameCnt;
        RealSide = board.side;
      }
    }
  }
}

/*
 * Extracts a command from the engine input buffer.
 *
 * The command is removed from the buffer.
 * If the command is a move, the move is made.
 */
void NextEngineCmd( void )
{
  char engineinput[BUF_SIZE]="";
  char enginemovestr[BUF_SIZE]="";
  leaf* enginemove;

  if ( strlen( engineinputbuf ) > 0 ) {
    if ( GetNextLine( engineinputbuf, engineinput ) > 0 ) {
      dbg_printf("< ENGINE: %s\n", engineinput);
      if ( strncmp( engineinput, "move", 4 ) == 0 ) {
        /* Input from engine is a move */
        sscanf( engineinput, "move %s", enginemovestr );
        enginemove = ValidateMove( enginemovestr );
        if ( enginemove == (leaf *) NULL ) {
          dbg_printf( "Bad move from engine\n" );
        } else {
          dbg_printf( "Engine move: <%s> (%d,%d)\n", enginemovestr,
                      (enginemove!=NULL ? enginemove->move : -1),
                      (enginemove!=NULL ? enginemove->score : -1) );
          SANMove (enginemove->move, 1);
          MakeMove( board.side, &(enginemove->move) );
          strcpy (Game[GameCnt].SANmv, SANmv);
	  if ( !(flags & XBOARD) ) {
		  ShowBoard();
                  dbg_printf("USER <: My move is : %s\n", SANmv);
		  printf( _("\nMy move is : %s\n"), SANmv );
                  fflush( stdout );
	  } else {
                  dbg_printf("USER <: %d. ... %s\n", GameCnt/2 + 1, enginemovestr );
                  printf ("%d. ... %s\n", GameCnt/2 + 1, enginemovestr );
                  fflush( stdout );
                  dbg_printf("USER <: My move is : %s\n", enginemovestr);
		  printf( "My move is : %s\n", enginemovestr );
                  fflush( stdout );
          }
          RealGameCnt = GameCnt;
          showprompt = 1;
          /* Check if the color must be changed, e.g. after a go command. */
          if ( changeColor ) {
            RealGameCnt = GameCnt;
            RealSide = board.side;
          }
        }
      } else {
        dbg_printf( "USER <: %s\n",engineinput );
        printf( "%s", engineinput );
        if ( flags & XBOARD ) {
          fflush( stdout );
        }
      }
    }
  }
}

/*
 * Stores in a global flag variable whether an input from the user is a
 * valid move (1) or not (0).
 */
void SetUserInputValidMove( int valid )
{
  userInputIsAMove = valid;
}

/*
 * Return value:
 *      1 - user input is a valid move
 *      0 - user input is not a valid move
 */
static int UserInputIsAValidMove(void)
{
  return ( userInputIsAMove );
}

/*
 * Extracts a line from 'buf' and copies it to 'line'.
 *
 * The end-of-line character is also copied.
 */
static int GetNextLine( char buf[], char line[] )
{
  char bufaux[BUF_SIZE]="";
  int i=0;
  int found=0;

  for ( i=0; i<strlen( buf ); ++i ) {
    line[i] = buf[i];
    line[i+1] = '\0';
    if ( line[i] == '\n' ) {
      found = 1;
      strcpy( bufaux, buf );
      strcpy( buf, zerochar );
      if ( strlen( &(bufaux[i+1]) ) > 0 ) {
        strcpy( buf, &(bufaux[i+1]) );
      }
      break;
    }
  }
  return found;
}

/*
 * Gets a line from 'buf' and copies it to 'line'.
 * Unlike GetNextLine, it will not remove the line from the
 * original buffer.
 *
 * The end-of-line character is also copied.
 */
static int GetNextLineNoRemove( char buf[], char line[] )
{
  int i=0;
  int found=0;

  for ( i=0; i<strlen( buf ); ++i ) {
    line[i] = buf[i];
    line[i+1] = '\0';
    if ( line[i] == '\n' ) {
      found = 1;
      break;
    }
  }
  return found;
}

/*
 * Flags whether the color must be changed, e.g. due to an undo command.
 */
void ChangeColor( int change )
{
  changeColor = change;
}

/*
 * Sets the autoGo flag, meaning that after a user move a go command will be
 * sent to the engine. This may be necessary after an undo.
 */
void SetAutoGo( int go )
{
  autoGo = go;
}

/*
 * Checks whether the autoGo flag is set or not.
 */
int GetAutoGo( void )
{
  return autoGo;
}

/*
 * Solves a position given in an EPD file.
 */
void SolvePosition( char move[], const char position[] )
{
  char msg[BUF_SIZE]="";
  char engineinput[BUF_SIZE]="";
  int solved = 0;

  /* TODO Translatable or not? */
  printf( "\nSolve position:\n\t%s\n", position );

  /* Send position to adapter/engine. */
  strcpy( msg, "setboard " );
  strcat( msg, position );
  msg[strlen(msg)-1] = '\0';
  SendToEngine( msg );

  /* Set adapter/engine to analyse. */
  sprintf( msg, "st %d\ngo", (int)round( SearchTime ) );
  SendToEngine( msg );

  /* Read adapter/engine's answer (the move). */
  while ( ( ! ReadFromEngine() ) || ( ! solved ) ) {
    if ( strlen( engineinputbuf ) > 0 ) {
      GetNextLineNoRemove( engineinputbuf, engineinput );
      assert( strlen( engineinput ) > 0 );
      if ( strncmp( engineinput, "move", 4 ) == 0 ) {
        solved = 1;
        break;
      } else {
        GetNextLine( engineinputbuf, engineinput );
        printf( "%s\n", engineinput );
      }
    }
    sleep( 0 );
  }
  NextEngineCmd();

  strcpy( move, SANmv );
}

/*
 * Reads a char string entered by the user.
 * The string must be a command or a move.
 * The string is sent to the engine straight away.
 */
void ForwardUserInputToEngine( void )
{
  int nread=0;
  fd_set set[1];
  struct timeval time_val[1];
  int userinputready=0;
  char userinputaux[BUF_SIZE]="";

  /* Poll input from user in non-blocking mode */
  FD_ZERO(set);
  FD_SET(STDIN_FILENO,set);
  time_val->tv_sec = 0;
  time_val->tv_usec = 0;
  userinputready = select( STDIN_FILENO+1, set, NULL, NULL, time_val );

  if ( userinputready == -1 ) {
    printf( "Error reading user input.\n" );
  } else if ( userinputready > 0 ) {
    /* There are some data from the user. Read the data */
    strncpy( userinputaux, zerochar, BUF_SIZE );
    nread = read( STDIN_FILENO, userinputaux, BUF_SIZE );
    /* Send the data to the engine */
    assert( nread+1 < BUF_SIZE-1 );
    if ( strcmp(userinputaux,"quit") == 0 || strcmp(userinputaux,"quit\n") == 0 ) {
	  SET (flags, QUIT);
    }
    userinputaux[nread] = '\n';
    userinputaux[nread+1] = '\0';
    int outError=0;
    int msg_count=0;
    msg_count = write( pipefd_a2e[1], userinputaux, nread+1 );
    if (msg_count == -1)
      outError = errno;
    else
      outError = 0;
    if ( ! ( ( msg_count == nread+1 ) && ( outError == 0 ) ) ) {
        printf( "Error sending message to engine.\n" );
        assert( 0 );
    }
  }
}

/*
 * Reads a char string message from the engine.
 * The message is output'd straight away.
 */
void ForwardEngineOutputToUser( void )
{

  int nread=0;
  fd_set set[1];
  struct timeval time_val[1];
  int engineinputready=0;
  char engineinputaux[BUF_SIZE]="";

  /* Poll input from engine in non-blocking mode */
  FD_ZERO(set);
  FD_SET( pipefd_e2a[0],set);
  time_val->tv_sec = 0;
  time_val->tv_usec = 0;
  engineinputready = select( pipefd_e2a[0]+1, set, NULL, NULL, time_val );

  if ( engineinputready == -1 ) {
    printf( "Error reading engine input.\n" );
  } else if ( engineinputready > 0 ) {
    /* There are some data from the engine. Read the data */
    strncpy( engineinputaux, zerochar, BUF_SIZE );
    nread = read( pipefd_e2a[0], engineinputaux, BUF_SIZE );
    /* Write data to output */
    assert( nread < BUF_SIZE-1 );
    engineinputaux[nread] = '\0';
    write( STDOUT_FILENO, engineinputaux, nread );
  }
}
