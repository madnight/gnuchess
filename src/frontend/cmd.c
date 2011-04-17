/* cmd.c

   GNU Chess frontend

   Copyright (C) 2001-2011 Free Software Foundation, Inc.

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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "version.h"
#include "common.h"

static char logfile[MAXSTR];
static char gamefile[MAXSTR];

/*
 * Splitting input is actually not neccessary, but we find
 * tokens separated by whitespace and put pointers on them.
 * How many do we need? We just take 3 for now. Check if the
 * fact that tokens are not 0-terminated but whitespace-terminated
 * generates bugs. (Already killed one bug in move.c)
 * We also kill trailing whitespace. (The trailing '\n' might
 * be really annoying otherwise.)
 */

#define TOKENS 3

static char *token[TOKENS];
char *endptr;

static int hardFlag=0;
static int postFlag=0;

static void split_input(void)
{
  /* r points to the last non-space character */
  char *s, *r;
  int k;

  for (k = 0, s = r = inputstr; k < TOKENS; ++k) {
    /* Skip leading whitespace */
    while (isspace(*s)) s++;
    token[k] = s;
    /* Skip token */
    while (*s && !isspace(*s)) r = s++;
  }
  while (*s) {
    while (isspace(*s)) s++;
    while (*s && !isspace(*s)) r = s++;
  }
  r[1] = '\0';
}

/*
 * Compares two tokens, returns 1 on equality. Tokens
 * are separated by whitespace.
 */
static int tokeneq(const char *s, const char *t)
{
  while (*s && *t && !isspace(*s) && !isspace(*t)) {
    if (*s++ != *t++) return 0;
  }
  return (!*s || isspace(*s)) && (!*t || isspace(*t));
}
  
void cmd_accepted(void) 
{
  SetDataToEngine( token[0] );
}

void cmd_activate(void) 
{
  printf( "'activate' not currently supported\n" );
}
 
void cmd_analyze(void)
{
  /*
   * "analyze" mode is similar to force, hard and post together
   * in that it produces a text output like post, but must
   * think indefinitely like ponder.
   *
   * Some additional output is expected in command ".\n" but if ignored
   * this should not be sent any more
   */

/* TODO correct output, add fail high low */

  SET (flags, ANALYZE);
  SetDataToEngine( "hard\npost\nanalyze" );
}

void cmd_bk(void)
{
  SetDataToEngine( "bk" );
}

void cmd_black(void) 
{
 /* 
  * No longer used by Xboard but requested as a feature
  */

  printf( "'black' not currently supported\n" );
}

void cmd_book(void)
{
  char data[MAXSTR]="";
  strcpy( data, "book " );
  if (tokeneq(token[1], "add")) {
    if (access(token[2], F_OK) < 0) {
      printf("The syntax to add a new book is:\n\n\tbook add file.pgn\n");
    } else {
      strcat( data, "add " );
      strcat( data, token[2] );
    }
  } else if (tokeneq (token[1], "on") || tokeneq(token[1], "prefer")) {
    strcpy( data, "book on" );
    printf( "'book on' not currently supported. Use polyglot.ini.\n" );
    /*printf("book now on.\n");*/
  } else if (tokeneq (token[1], "off")) {
    strcpy( data, "book off" );
    printf( "'book off' not currently supported. Use polyglot.ini.\n" );
    /*printf("book now off.\n");*/
  } else if (tokeneq (token[1], "best")) {
    strcpy( data, "book best" );
    printf( "'book best' not currently supported. Use polyglot.ini.\n" );
    /*printf("book now best.\n");*/
  } else if (tokeneq (token[1], "worst")) {
    strcpy( data, "book worst" );
    printf( "'book worst' not currently supported. Use polyglot.ini.\n" );
    /*printf("book now worst.\n");*/
  } else if (tokeneq (token[1], "random")) {
    strcpy( data, "book random" );
    printf( "'book random' not currently supported. Use polyglot.ini.\n" );
    /*printf("book now random.\n");*/
  } else {
    printf( "Incorrect book option\n" );
    return;
  }
  SetDataToEngine( data );
}

void cmd_depth(void)
{
  char data[MAXSTR]="";
  int searchDepth=0;
  searchDepth = atoi( token[1] );
  sprintf( data, "sd %d", searchDepth );
  SetDataToEngine( data );
  printf("Search to a depth of %d\n",searchDepth);
}

void cmd_easy(void)
{
  SetDataToEngine( token[0] );
}

/* Predecessor to setboard */
void cmd_edit(void) 
{
  if ( flags & XBOARD ) {
    printf("tellusererror command 'edit' not implemented\n");
    fflush(stdout);
  }
}

void cmd_exit(void) 
{ 
  /*
   * "exit" is a synonym for quit except in engine mode
   * when it means leave analyze mode
   */

  if ( flags & ANALYZE ){
    flags = preanalyze_flags ; /* this implicitly clears ANALYZE flag */
    SetDataToEngine( token[0] );
  } else {
    cmd_quit(); 
  }
   

}

void cmd_force(void)
{
  SET (flags, MANUAL);
  SetDataToEngine( token[0] );
}

void cmd_go(void)
{
  SET (flags, THINK);
  CLEAR (flags, MANUAL);
  CLEAR (flags, TIMEOUT);
  CLEAR (flags, ENDED);
  computer = board.side;
  ExpectAnswerFromEngine( true );
  ChangeColor( true );
  SetDataToEngine( token[0] );
}

void cmd_hard(void)
{
  SetDataToEngine( token[0] );
}

void cmd_hash(void)
{
  /* TODO: to be re-implemented based on Polyglot */
  printf( "'hash' not currently supported\n" );
}

void cmd_hashsize(void)
{
  /* TODO: to be re-implemented based on Polyglot */
  printf( "'hashsize' not current supported\n" );
}

/* Give a possible move for the player to play */
void cmd_hint(void)
{
  SetDataToEngine( token[0] );
  /* TODO if no hint, inform on stdout */
}

void cmd_ics(void)
{
  SetDataToEngine( token[0] );
}

void cmd_level(void)
{
  SetDataToEngine( token[0] );
  SearchDepth = 0;
  sscanf (token[1], "%d %f %d", &TCMove, &TCTime, &TCinc);
  if (TCMove == 0) {
    TCMove =  35 /* MIN((5*(GameCnt+1)/2)+1,60) */;
    printf("TCMove = %d\n",TCMove);
  }
  if (TCTime == 0) {
    SearchTime = TCinc / 2.0f ;
    printf("Fischer increment of %d seconds\n",TCinc);
  } else {
    MoveLimit[white] = MoveLimit[black] = TCMove - (GameCnt+1)/2;
    TimeLimit[white] = TimeLimit[black] = TCTime * 60;
    if (!(flags & XBOARD)) {
      printf ("Time Control: %d moves in %.2f secs\n", 
	      MoveLimit[white], TimeLimit[white]);
      printf("Fischer increment of %d seconds\n",TCinc);
    }
  }
}

void cmd_list(void)
{
  if (token[1][0] == '?') {
    printf("name    - list known players alphabetically\n");
    printf("score   - list by GNU best result first \n");
    printf("reverse - list by GNU worst result first\n");
  } else {
    if (token[1][0] == '\0') DBListPlayer("rscore");
    else DBListPlayer(token[1]);
  }
}

void cmd_load(void)
{
  char data[MAXSTR]="";
  LoadEPD (token[1]);
  if (!ValidateBoard()) {
    SET (flags, ENDED);
    printf ("Board is wrong!\n");
  } else {
    /* Read EPD file and send contents to engine */
    FILE *epdfile = fopen( token[1], "r" );
    char epdline[MAXSTR]="";
    if ( epdfile == NULL ) {
      printf("Error reading file '%s'\n", token[1] );
    } else {
      if ( fgets( epdline, MAXSTR, epdfile ) == NULL ) {
        printf("Error reading file '%s'\n", token[1] );
      } else {
        strcpy( data, "setboard " );
        int i=0;
        while ( epdline[i] != '\n' ) {
          data[i+9] = epdline[i];
          ++i;
        }
        data[i+9] = '\0';
        SetDataToEngine( data );
        SetAutoGo( true );
      }
    }
  }
}

void cmd_manual(void)
{
  SetDataToEngine( "force" );
  SET (flags, MANUAL); 
}

/* Move now, not applicable */
void cmd_movenow(void)
{
  SetDataToEngine( "?" );
}

/*
 * TODO: Add a logpath variable or macro, not always dump into current
 * dir. Again, how does one handle paths portably across Unix/Windows?
 *   -- Lukas 
 */
void cmd_name(void)
{
  SetDataToEngine( token[0] );
  int suffix = 0;

  /* name[sizeof name - 1] should always be 0 */
  strncpy(name, token[1], sizeof name - 1);
  for (suffix = 0; suffix < 1000; suffix++) {
    sprintf(logfile,"log.%03d",suffix);
    sprintf(gamefile,"game.%03d",suffix);
    /*
     * There is an obvious race condition here but who cares, we just
     * bail out in case of failure... --Lukas 
     */
    if (access(logfile,F_OK) < 0) {
      ofp = fopen(logfile,"w");
      if (ofp == NULL) {
	ofp = stdout;
	fprintf(stderr, "Failed to open %s for writing: %s\n", 
		logfile, strerror(errno));
      }
      return;
    }
  }
  fprintf(stderr, "Could not create logfile, all slots occupied.\n");
  fprintf(stderr, "You may consider deleting or renaming your existing logfiles.\n");
}

void cmd_new(void)
{
  InitVars ();
  NewPosition ();
  /* Protocol specification for ANALYZE says "new" does not end analysis */
  if (!(flags & ANALYZE))
    CLEAR (flags, MANUAL);
  CLEAR (flags, THINK);
  myrating = opprating = 0;
  SetDataToEngine( token[0] );
}

void cmd_nopost(void)
{
  CLEAR (flags, POST);
  postFlag = 0;
  SetDataToEngine( token[0] );
}

void cmd_null(void)
{
  printf( "'null' not currently supported\n" );
}

void cmd_otim(void)
{
   SetDataToEngine( token[0] );
}

void cmd_pgnload(void)
{
  char tmp_epd[]=".tmp.epd";
  char data[MAXSTR]="";
  FILE *epdfile=NULL;
  char epdline[MAXSTR]="";

  PGNReadFromFile (token[1]);
  SaveEPD( tmp_epd );
  epdfile = fopen( tmp_epd, "r" );
  if ( fgets( epdline, MAXSTR, epdfile ) == NULL ) {
    printf( "Incorrect epd file\n" );
    return;
  }
  strcpy( data, "setboard " );
  int i=0;
  while ( epdline[i] != '\n' ) {
    data[i+9] = epdline[i];
    ++i;
  }
  data[i+9] = '\0';
  SetDataToEngine( data );
  SetAutoGo( true );
}

/*
 * XXX - Filenames with spaces will break here,
 * do we want to support them? I vote for "no" 
 *   - Lukas
 */
void cmd_pgnsave(void)
{
  if ( strlen(token[1]) > 0 )
    PGNSaveToFile (token[1], "");
  else
    printf("Invalid filename.\n");
}
 
void cmd_ping(void)
{
  SetDataToEngine( token[0] );
  /* If ping is received when we are on move, we are supposed to 
     reply only after moving.  In this version of GNU Chess, we
     never read commands while we are on move, so we don't have to
     worry about that here. */
  printf("pong %s\n", token[1]);
  fflush(stdout);
}
 
void cmd_post(void)
{
  SET (flags, POST);
  postFlag = 1;
  if ( hardFlag && postFlag )
    ExpectAnswerFromEngine( true );
  ExpectAnswerFromEngine( true );
  SetDataToEngine( token[0] );
}

void cmd_protover(void)
{
  SetDataToEngine( token[0] );
  return;
  if (flags & XBOARD) {
    /* Note: change this if "draw" command is added, etc. */
    printf("feature setboard=1 analyze=1 ping=1 draw=0 sigint=0"
	   " variants=\"normal\" myname=\"%s %s\" done=1\n",
	   PROGRAM, VERSION);
    fflush(stdout);
  }
}

void cmd_quit(void) { SET (flags, QUIT); }

void cmd_random(void)
{
  SetDataToEngine( token[0] );
}

void cmd_rating(void)
{
  myrating = atoi(token[1]);
  opprating = atoi(token[2]);
  fprintf(ofp,"my rating = %d, opponent rating = %d\n",myrating,opprating); 
  /* Change randomness of book based on opponent rating. */
  /* Basically we play narrower book the higher the opponent */
  if (opprating >= 1700) bookfirstlast = 2;
  else if (opprating >= 1700) bookfirstlast = 2;
  else bookfirstlast = 2;
}

void cmd_rejected(void) {}

void cmd_remove(void)
{
  SetDataToEngine( token[0] );
  if (GameCnt >= 0) {
    CLEAR (flags, ENDED);
    CLEAR (flags, TIMEOUT);
    UnmakeMove (board.side, &Game[GameCnt].move);
    if (GameCnt >= 0) {
      UnmakeMove (board.side, &Game[GameCnt].move);
      if (!(flags & XBOARD))
           ShowBoard ();
    }
    PGNSaveToFile ("game.log","");
  } else
    printf ("No moves to undo! \n");
}

void cmd_result(void)
{
  SetDataToEngine( token[0] );
  if (ofp != stdout) {  
    fprintf(ofp, "result: %s\n",token[1]);
    fclose(ofp); 
    ofp = stdout;
    printf("Save to %s\n",gamefile);
    PGNSaveToFile (gamefile, token[1]);
    DBUpdatePlayer (name, token[1]);
  }
}
	
void cmd_save(void)
{  
  if ( strlen(token[1]) > 0 )
    SaveEPD (token[1]);
  else
    printf("Invalid filename.\n");
}

void cmd_setboard(void)
{
  /* setboard uses FEN, not EPD, but ParseEPD will accept FEN too */
  ParseEPD (token[1]);
  NewPosition();
}

void cmd_solve(void) { Solve (token[1]); }

/* Set total time for move to be N seconds is "st N" */
void cmd_st(void) 
{
  char data[MAXSTR]="";
  /* Approximately level 1 0 N */
  sscanf(token[1],"%d",&TCinc);
  /* Allow a little fussiness for failing low etc */
  SearchTime = TCinc * 0.90f ;
  sprintf( data, "st %d", atoi( token[1] ) );
  SetDataToEngine( data );
}

void cmd_switch(void)
{
  board.side = 1^board.side;
  board.ep = -1 ; /* Enpassant doesn't apply after switch */
  printf ("%s to move\n", board.side == white ? "White" : "Black");
}

void cmd_time(void)
{
  SetDataToEngine( token[0] );
  printf( "Old TimeLimit = %g\n", TimeLimit[1^board.side] );
  TimeLimit[1^board.side] = atoi(token[1]) / 100.0f ;
  printf( "New TimeLimit = %g\n", TimeLimit[1^board.side] );
}

void cmd_undo(void)
{
  SetDataToEngine( "force\nundo" );
  ChangeColor( true );
  SetAutoGo( true );
  if (GameCnt >= 0)
    UnmakeMove (board.side, &Game[GameCnt].move);
  else
    printf ("No moves to undo! \n");
  MoveLimit[board.side]++;
  TimeLimit[board.side] += Game[GameCnt+1].et;
  if (!(flags & XBOARD)) ShowBoard ();
}

void cmd_usage(void) 
{
      printf (
     "\n"
     "Usage: %s [OPTION]...\n"
     "\n"
     "Play the game of chess\n"
     "\n"
     "Options:\n"
     " -h, --help         display this help and exit\n"
     " -v, --version      display version information and exit\n" 
     " -q, --quiet        make the program silent on startup\n"
     "     --silent       same as -q\n"
     "\n"
     " -x, --xboard       start in engine mode\n"
     " -p, --post         start up showing thinking\n"
     " -e, --easy         disable thinking in opponents time\n"
     " -m, --manual       enable manual mode\n"
     " -u, --uci          enable UCI protocol (externally behave as UCI engine)\n"
     " -s size, --hashsize=size   specify hashtable size in slots\n"
     "\n"
     " Options xboard and post are accepted without leading dashes\n"
     " for backward compatibility.\n"
     "\n"
     " Moves are accepted either in standard algebraic notation (SAN) or\n"
     " in coordinate algebraic notation.\n"
     "\n"
     "Report bugs to <bug-gnu-chess@gnu.org>.\n"
     "\n", progname);
     }


/* Play variant, we instruct interface in protover we play normal */
void cmd_variant(void) {}

void cmd_usermove(void)
{
  /* TODO: Remove the first SetDataToEngine */
  /*SetDataToEngine( token[0] );*/
   leaf *ptr; 
   ptr = ValidateMove (token[1]);
   if (ptr != NULL) {
     /* Since the user entered a move:
      * 1. The move must be sent to the engine.
      * 2. A reply is expected from the engine.
      */
     SetUserInputValidMove( 1 );
     SetDataToEngine( token[0] );
     ExpectAnswerFromEngine( true );
     SANMove (ptr->move, 1);
     MakeMove (board.side, &ptr->move);
     strcpy (Game[GameCnt].SANmv, SANmv);
     printf("%d. ",GameCnt/2+1);
     printf("%s",token[1]);
     if (ofp != stdout) {
       fprintf(ofp,"%d. ",GameCnt/2+1);
       fprintf(ofp,"%s",token[1]);
     }
     putchar('\n');
     fflush(stdout);
     if (ofp != stdout) {
       fputc('\n',ofp);
       fflush(ofp);
     }
     if (!(flags & XBOARD)) ShowBoard (); 
     SET (flags, THINK);
   }
   else {
     /*
      * Must Output Illegal move to prevent Xboard accepting illegal
      * en passant captures and other subtle mistakes
      */
     printf("Invalid move: %s\n",token[1]);
     fflush(stdout);
   }  
}

void cmd_version(void)
{
   if (!(flags & XBOARD))
     printf ("%s %s\n", PROGRAM, VERSION);
   else
     printf ("Chess\n");
}

void cmd_white(void) 
{
 /* 
  * No longer used by Xboard but requested as a feature
  */
  printf( "'white' not currently supported\n" );
}

void cmd_xboard(void)
{
  SetDataToEngine( "xboard" );
  if (tokeneq (token[1], "off"))
    CLEAR (flags, XBOARD);
  else if (tokeneq (token[1], "on"))
    SET (flags, XBOARD);
  else if (!(flags & XBOARD)) { /* set if unset and only xboard called */
    SET (flags, XBOARD);	    /* like in xboard/winboard usage */
  }
}

/*
 * Command with subcommands, could write secondary method
 * tables here
 */

void cmd_show (void)
/************************************************************************
 *
 *  The show command driver section.
 *
 ************************************************************************/
{
   if (tokeneq (token[1], "board"))
      ShowBoard ();
   else if (tokeneq (token[1], "rating"))
   {
      printf("My rating = %d\n",myrating);
      printf("Opponent rating = %d\n",opprating);
   } 
   else if (tokeneq (token[1], "time"))
      ShowTime ();
   else if (tokeneq (token[1], "moves")) {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenMoves (1);      
      ShowMoveList (1);
      printf ("No. of moves generated = %ld\n", GenCnt);
   }
   else if (tokeneq (token[1], "escape")) {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenCheckEscapes (1);      
      ShowMoveList (1);
      printf ("No. of moves generated = %ld\n", GenCnt);
   }
   else if (tokeneq (token[1], "noncapture"))
   {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenNonCaptures (1);      
      FilterIllegalMoves (1);
      ShowMoveList (1);
      printf ("No. of moves generated = %ld\n", GenCnt);
   }
   else if (tokeneq (token[1], "capture"))
   {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenCaptures (1);      
      FilterIllegalMoves (1);
      ShowMoveList (1);
      printf ("No. of moves generated = %ld\n", GenCnt);
   }
   else if (tokeneq (token[1], "eval") || tokeneq (token[1], "score"))
   {
      printf( "'show eval/score' not currently supported\n" );
      return;
   }
   else if (tokeneq (token[1], "game"))
     ShowGame ();
   else if (tokeneq (token[1], "pin"))
   {
      printf( "'show pin' not currently supported\n" );
      return;
   }
}

void cmd_test (void)
/*************************************************************************
 *
 *  The test command driver section.
 *
 *************************************************************************/
{
  printf( "'test' not currently supported\n" );
}

/*
 * This is more or less copied from the readme, and the
 * parser is not very clever, so the lines containing
 * command names should not be indented, the lines with
 * explanations following them should be indented. Do not
 * use tabs for indentation, only spaces. CAPITALS are
 * reserved for parameters in the command names. The
 * array must be terminated by two NULLs.
 * 
 * This one should be integrated in the method table.
 * (Very much like docstrings in Lisp.)
 */

static const char * const helpstr[] = {
   "^C",
   " Typically the interrupt key stops a search in progress,",
   " makes the move last considered best and returns to the",
   " command prompt",
   "quit",
   " quit the program.",
   "exit",
   " In analysis mode this stops analysis, otherwise it quits the program.",
   "help",
   " Produces a help blurb corresponding to this list of commands.",
   "book",
   " add - compiles book.dat from book.pgn",
   " on - enables use of book",
   " off - disables use of book",
   " worst - play worst move from book",
   " best - play best move from book",
   " prefer - default, same as 'book on'",
   " random - play any move from book",
   "version",
   " prints out the version of this program",
   "pgnsave FILENAME",
   " saves the game so far to the file from memory",
   "pgnload FILENAME",
   " loads the game in the file into memory",
   "force",
   "manual",
   " Makes the program stop moving. You may now enter moves",
   " to reach some position in the future.",
   " ",
   "white",
   " Program plays white",
   "black",
   " Program plays black",
   "go",
   " Computer takes whichever side is on move and begins its",
   " thinking immediately",
   "post",
   " Arranges for verbose thinking output showing variation, score,",
   " time, depth, etc.",
   "nopost",
   " Turns off verbose thinking output",
   "name NAME",
   " Lets you input your name. Also writes the log.nnn and a",
   " corresponding game.nnn file. For details please see",
   " auxillary file format sections.",
   "result",
   " Mostly used by Internet Chess server.",
   "activate",
   " This command reactivates a game that has been terminated automatically",
   " due to checkmate or no more time on the clock. However, it does not",
   " alter those conditions. You would have to undo a move or two or",
   " add time to the clock with level or time in that case.",
   "rating COMPUTERRATING OPPONENTRATING",
   " Inputs the estimated rating for computer and for its opponent",
   "new",
   " Sets up new game (i.e. positions in original positions)",
   "time",
   " Inputs time left in game for computer in hundredths of a second.",
   " Mostly used by Internet Chess server.",
   "hash",
   " on - enables using the memory hash table to speed search",
   " off - disables the memory hash table",
   "hashsize N",
   " Sets the hash table to permit storage of N positions",
   "null",
   " on - enables using the null move heuristic to speed search",
   " off - disables using the null move heuristic",
   "xboard",
   " on - enables use of xboard/winboard",
   " off - disables use of xboard/winboard",
   "depth N",
   " Sets the program to look N ply (half-moves) deep for every",
   " search it performs. If there is a checkmate or other condition",
   " that does not allow that depth, then it will not be ",
   "level MOVES MINUTES INCREMENT",
   " Sets time control to be MOVES in MINUTES with each move giving",
   " an INCREMENT (in seconds, i.e. Fischer-style clock).",
   "load",
   "epdload",
   " Loads a position in EPD format from disk into memory.",
   "save",
   "epdsave",
   " Saves game position into EPD format from memory to disk.",
   "switch",
   " Switches side to move",
   "solve FILENAME",
   "solveepd FILENAME",
   " Solves the positions in FILENAME",
   "remove",
   " Backs up two moves in game history",
   "undo",
   " Backs up one move in game history",
   "usage",
   " Display command line syntax",
   "show",
   " board - displays the current board",
   " time - displays the time settings",
   " moves - shows all moves using one call to routine",
   " escape - shows moves that escape from check using one call to routine",
   " noncapture - shows non-capture moves",
   " capture - shows capture moves",
   " eval [or score] - shows the evaluation per piece and overall",
   " game - shows moves in game history",
   " pin - shows pinned pieces",
   "test",
   " movelist - reads in an epd file and shows legal moves for its entries",
   " capture - reads in an epd file and shows legal captures for its entries",
   " movegenspeed - tests speed of move generator",
   " capturespeed - tests speed of capture move generator",
   " eval - reads in an epd file and shows evaluation for its entries",
   " evalspeed tests speed of the evaluator",
   "bk",
   " show moves from opening book.",
   NULL,
   NULL
};

void cmd_help (void)
/**************************************************************************
 *
 *  Display all the help commands.
 *
 **************************************************************************/
{
   const char * const *p;
   int count, len;

   if (strlen(token[1])>0) {
      for (p=helpstr, count=0; *p; p++) {
	 if  (strncmp(*p, token[1], strlen(token[1])) == 0) {
	    puts(*p);
	    while (*++p && **p != ' ') /* Skip aliases */ ;
	    for (; *p && **p == ' '; p++) {
	       puts(*p);
	    }
	    return;
	 }
      }
      printf("Help for command %s not found\n\n", token[1]);
   }
   printf("List of commands: (help COMMAND to get more help)\n");
   for (p=helpstr, count=0; *p; p++) {
      len = strcspn(*p, " ");
      if (len > 0) {
	 count += printf("%.*s  ", len, *p);
	 if (count > 60) {
	    count = 0;
	    puts("");
	 }
      }
   }
   puts("");
}

/*
 * Try a method table, one could also include the documentation
 * strings here
 */

struct methodtable {
  const char *name;
  void (*method) (void);
};

/* Last entry contains to NULL pointers */

/* List commands we don't implement to avoid illegal moving them */

const struct methodtable commands[] = {
  { "?", cmd_movenow },
  { "accepted", cmd_accepted },
  { "activate", cmd_activate },
  { "analyze", cmd_analyze },
  { "bk", cmd_bk },
  { "black", cmd_black },
  { "book", cmd_book },
  { "depth", cmd_depth },
  { "easy", cmd_easy },
  { "edit", cmd_edit },
  { "epdload", cmd_load },
  { "epdsave", cmd_save },
  { "exit", cmd_exit },
  { "force", cmd_force },
  { "go", cmd_go },
  { "hard", cmd_hard },
  { "hash", cmd_hash },
  { "hashsize", cmd_hashsize },
  { "help", cmd_help },
  { "hint", cmd_hint },
  { "ics", cmd_ics },
  { "level", cmd_level },
  { "list", cmd_list },
  { "load", cmd_load },
  { "manual", cmd_manual },
  { "name", cmd_name },
  { "new", cmd_new },
  { "nopost", cmd_nopost },
  { "null", cmd_null },
  { "otim", cmd_otim },
  { "pgnload", cmd_pgnload },
  { "pgnsave", cmd_pgnsave },
  { "ping", cmd_ping },
  { "post", cmd_post },
  { "protover", cmd_protover },
  { "quit", cmd_quit },
  { "random", cmd_random },
  { "rating", cmd_rating },
  { "rejected", cmd_rejected },
  { "remove", cmd_remove },
  { "result", cmd_result },
  { "save", cmd_save },
  { "setboard", cmd_setboard },
  { "show", cmd_show },
  { "solve", cmd_solve },
  { "solveepd", cmd_solve },
  { "st", cmd_st },
  { "switch", cmd_switch },
  { "test", cmd_test },
  { "time", cmd_time },
  { "undo", cmd_undo },
  { "usage", cmd_usage },
  { "usermove", cmd_usermove },
  { "variant", cmd_variant },
  { "version", cmd_version },
  { "white", cmd_white },
  { "xboard", cmd_xboard },
  { NULL, NULL }
};

void parse_input(void)
/*************************************************************************
 *
 *  This is the main user command interface driver.
 *
 *************************************************************************/
{
   leaf *ptr; 
   const struct methodtable * meth;
 
   dbg_printf("parse_input() called, inputstr = *%s*\n", inputstr);

   /* Initialize variables used to send messages to the engine */ 
   SetDataToEngine( "" );
   ExpectAnswerFromEngine( false );
   SetUserInputValidMove( 0 );
   ChangeColor( false );

   split_input();
  
   for (meth = commands; meth->name != NULL; meth++) {
     if (tokeneq(token[0], meth->name)) {
       meth->method();
       return;
     }
   }

   /* OK, no known command, this should be a move */
   ptr = ValidateMove (token[0]);
   if (ptr != NULL) {
     /* Since the user entered a move:
      * 1. The move must be sent to the engine.
      * 2. A reply is expected from the engine.
      */
     SetUserInputValidMove( 1 );
     SetDataToEngine( token[0] );
     ExpectAnswerFromEngine( true );
     SANMove (ptr->move, 1);
     MakeMove (board.side, &ptr->move);
     strcpy (Game[GameCnt].SANmv, SANmv);
     printf("%d. ",GameCnt/2+1);
     printf("%s",token[0]);
     if (ofp != stdout) {
       fprintf(ofp,"%d. ",GameCnt/2+1);
       fprintf(ofp,"%s",token[0]);
     }
     putchar('\n');
     fflush(stdout);
     if (ofp != stdout) {
       fputc('\n',ofp);
       fflush(ofp);
     }
     if (!(flags & XBOARD)) ShowBoard (); 
     SET (flags, THINK);
   }
   else {
     /*
      * Must Output Illegal move to prevent Xboard accepting illegal
      * en passant captures and other subtle mistakes
      */
     printf("Invalid move: %s\n",token[0]);
     fflush(stdout);
   }  
}
