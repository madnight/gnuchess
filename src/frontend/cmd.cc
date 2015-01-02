/* cmd.c

   GNU Chess frontend

   Copyright (C) 2001-2013 Free Software Foundation, Inc.

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
#include "gettext.h"

#define _(str) gettext (str)

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
  printf( _("Command 'activate' is currently not supported.\n") );
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
  printf( _("Command 'black' is currently not supported.\n") );
}

void cmd_book(void)
{
  char data[MAXSTR]="";
  strcpy( data, "book " );
  if (tokeneq(token[1], "add")) {
    if (access(token[2], F_OK) < 0) {
      printf(_("The syntax to add a new book is:\n\n\tbook add file.pgn\n"));
    } else {
      strcat( data, "add " );
      strcat( data, token[2] );
    }
  } else if (tokeneq (token[1], "on") || tokeneq(token[1], "prefer")) {
    strcpy( data, "book on" );
    printf(_("book is now on\n"));
  } else if (tokeneq (token[1], "off")) {
    strcpy( data, "book off" );
    printf(_("book is now off\n"));
  } else if (tokeneq (token[1], "best")) {
    strcpy( data, "book best" );
    printf(_("book is now best\n"));
  } else if (tokeneq (token[1], "worst")) {
    strcpy( data, "book worst" );
    printf(_("book is now worst\n"));
  } else if (tokeneq (token[1], "random")) {
    strcpy( data, "book random" );
    printf(_("book is now random\n"));
  } else {
    printf( _("Incorrect book option\n") );
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
  printf(_("Search to a depth of %d\n"),searchDepth);
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
  pgnloaded = 0;
}

void cmd_hard(void)
{
  SetDataToEngine( token[0] );
}

void cmd_hash(void)
{
  if (tokeneq (token[1], "off")) {
    CLEAR (flags, USEHASH);
    SetDataToEngine( "hashoff" );
  } else if (tokeneq (token[1], "on")) {
    SET (flags, USEHASH);
    SetDataToEngine( "hashon" );
  }
  if ( flags & USEHASH ) {
    printf( _("Hashing is on.\n") );
  } else {
    printf( _("Hashing is off.\n") );
  }
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
  sscanf (token[1], "%d %f %d", &TCMove, &TCTime, &TCinc);
  if (TCMove == 0) {
    TCMove =  35 /* MIN((5*(GameCnt+1)/2)+1,60) */;
    printf("TCMove = %d\n",TCMove);
  }
  if (TCTime == 0) {
    SearchTime = TCinc / 2.0f ;
    printf(_("Fischer increment of %d seconds\n"),TCinc);
  } else {
    MoveLimit[white] = MoveLimit[black] = TCMove - (GameCnt+1)/2;
    TimeLimit[white] = TimeLimit[black] = TCTime * 60;
    if (!(flags & XBOARD)) {
      /* TRANSLATORS: Please be aware that the word 'move' is sometimes
         used as a synonym of 'ply', and sometimes in the sense of a
         full 2-ply move. */
      printf (_("Time Control: %d moves in %.2f secs\n"),
      MoveLimit[white], TimeLimit[white]);
      printf(_("Fischer increment of %d seconds\n"),TCinc);
    }
  }
}

void cmd_list(void)
{
  if (token[1][0] == '?') {
    printf(_("name    - list known players alphabetically\n"));
    printf(_("score   - list by GNU best result first \n"));
    printf(_("reverse - list by GNU worst result first\n"));
  } else {
    if (token[1][0] == '\0') DBListPlayer("rscore");
    else DBListPlayer(token[1]);
  }
}

void cmd_load(void)
{
  char data[MAXSTR]="";
  LoadEPD (token[1]);
  pgnloaded = 0;
  check_board();
  if (!ValidateBoard()) {
    SET (flags, ENDED);
    printf (_("Board is wrong!\n"));
  } else {
    /* Read EPD file and send contents to engine */
    FILE *epdfile = fopen( token[1], "r" );
    char epdline[MAXSTR]="";
    if ( epdfile == NULL ) {
      printf(_("Error reading file '%s'\n"), token[1] );
    } else {
      if ( fgets( epdline, MAXSTR, epdfile ) == NULL ) {
        printf(_("Error reading file '%s'\n"), token[1] );
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

void cmd_memory(void)
{
  if (token[1][0] == 0) {
    SetDataToEngine( "memory" );
    ExpectAnswerFromEngine( true );
  } else {
    unsigned int memory;
    if ( sscanf( token[1], "%d", &memory ) == 1 ) {
      char data[MAXSTR]="";
      sprintf( data, "memory %d\nmemory", memory );
      SetDataToEngine( data );
    }
  }
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
        fprintf(stderr, _("Failed to open %s for writing: %s\n"),
        logfile, strerror(errno));
      }
      return;
    }
  }
  fprintf(stderr, _("Could not create logfile, all slots occupied.\n"));
  fprintf(stderr, _("You may consider deleting or renaming your existing logfiles.\n"));
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
  if (tokeneq (token[1], "off")) {
    CLEAR (flags, USENULL);
    SetDataToEngine( "nulloff" );
  } else if (tokeneq (token[1], "on")) {
    SET (flags, USENULL);
    SetDataToEngine( "nullon" );
  }
  if ( flags & USENULL ) {
    printf( _("Null-move heuristic is on.\n") );
  } else {
    printf( _("Null-move heuristic is off.\n") );
  }
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

  PGNReadFromFile (token[1],0);
  SaveEPD( tmp_epd );
  epdfile = fopen( tmp_epd, "r" );
  if ( fgets( epdline, MAXSTR, epdfile ) == NULL ) {
    printf( _("Incorrect epd file\n") );
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
  pgnloaded = 0;
}

void cmd_pgnreplay(void)
{
  char tmp_epd[]=".tmp.epd";
  char data[MAXSTR]="";
  FILE *epdfile=NULL;
  char epdline[MAXSTR]="";

  PGNReadFromFile (token[1],1);

  SaveEPD( tmp_epd );
  epdfile = fopen( tmp_epd, "r" );
  if ( fgets( epdline, MAXSTR, epdfile ) == NULL ) {
    printf( _("Incorrect epd file\n") );
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

  pgnloaded = 1;
  pgncnt = GameCnt;

  while (GameCnt >= 0) {
    if (GameCnt >= 0) {
      CLEAR (flags, ENDED);
      CLEAR (flags, TIMEOUT);
      ChangeColor( true );
      SetAutoGo( true );
      UnmakeMove (board.side, &Game[GameCnt].move);
      if (GameCnt >= 0) {
        UnmakeMove (board.side, &Game[GameCnt].move);
      }
    }
  }

  cmd_first();
}

void cmd_next(void)
{
  if (!pgnloaded)
    return;

  if ((GameCnt+1) <= pgncnt) {
    ChangeColor( true );
    SetAutoGo( true );
    MakeMove (board.side, &Game[GameCnt+1].move);
  } else {
    printf(_("No more moves. Game reached the end.\n"));
    return;
  }

  printf("%d. ",GameCnt/2+1);
  printf("%s\n", Game[GameCnt].SANmv);
  ShowBoard ();
}

void cmd_previous(void)
{
  if (!pgnloaded)
    return;

  if (GameCnt >= 0) {
    ChangeColor( true );
    SetAutoGo( true );
    UnmakeMove (board.side, &Game[GameCnt].move);
  }
  else {
    printf(_("Initial position reached. There are no earlier moves.\n"));
    return;
  }

  printf("%d. ",GameCnt/2+1);
  printf("%s\n", Game[GameCnt].SANmv);
  ShowBoard ();
}

void cmd_last(void)
{
  if (!pgnloaded)
    return;

  while (GameCnt+1 <= pgncnt) {
    ChangeColor( true );
    SetAutoGo( true );
    MakeMove (board.side, &Game[GameCnt+1].move);
  }

  printf("%d. ",GameCnt/2+1);
  printf("%s\n", Game[GameCnt].SANmv);
  ShowBoard ();
}

void cmd_first(void)
{
  if (!pgnloaded)
    return;

  while (GameCnt >= 0) {
    if (GameCnt >= 0) {
      CLEAR (flags, ENDED);
      CLEAR (flags, TIMEOUT);
      ChangeColor( true );
      SetAutoGo( true );
      UnmakeMove (board.side, &Game[GameCnt].move);
      if (GameCnt >= 0) {
        UnmakeMove (board.side, &Game[GameCnt].move);
      }
    }
  }

  ShowBoard ();
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
    printf(_("Invalid filename.\n"));
}

void cmd_graphic(void)
{
  graphicmodeoutput = 1;
  printf(_("Graphic mode is enabled.\n"));
}

void cmd_nographic(void)
{
  graphicmodeoutput = 0;
  printf(_("Graphic mode is disabled.\n"));
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
    printf("feature setboard=1 analyze=1 ping=1 draw=0 sigint=0\
 variants=\"normal\" myname=\"%s %s\" done=1\n",
      PROGRAM, VERSION);
    fflush(stdout);
  }
}

void cmd_quit(void) { SET (flags, QUIT); }

void cmd_random(void)
{
  printf( _("Command 'random' is currently not supported.\n") );
  SetDataToEngine( token[0] );
}

void cmd_rating(void)
{
  myrating = atoi(token[1]);
  opprating = atoi(token[2]);
  fprintf(ofp,_("my rating = %d, opponent rating = %d\n"),myrating,opprating);
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
    printf (_("No moves to undo!\n"));
}

void cmd_result(void)
{
  SetDataToEngine( token[0] );
  if (ofp != stdout) {
    fprintf(ofp, "result: %s\n",token[1]);
    fclose(ofp);
    ofp = stdout;
    printf(_("Save to %s\n"),gamefile);
    PGNSaveToFile (gamefile, token[1]);
    DBUpdatePlayer (name, token[1]);
  }
}

void cmd_save(void)
{
  if ( strlen(token[1]) > 0 )
    SaveEPD (token[1]);
  else
    printf(_("Invalid filename.\n"));
}

void cmd_setboard(void)
{
  /* setboard uses FEN, not EPD, but ParseEPD will accept FEN too */
  char data[MAXSTR]="";
  ParseEPD (token[1]);
  NewPosition();
  check_board();
  snprintf(data, sizeof(data), "setboard %s", token[1]);
  SetDataToEngine(data);
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
  printf( _("Command 'switch' is currently not supported.\n") );
}

void cmd_time(void)
{
  SetDataToEngine( token[0] );
  printf( _("Old TimeLimit = %g\n"), TimeLimit[1^board.side] );
  TimeLimit[1^board.side] = atoi(token[1]) / 100.0f ;
  printf( _("New TimeLimit = %g\n"), TimeLimit[1^board.side] );
}

void cmd_undo(void)
{
  SetDataToEngine( "force\nundo" );
  ChangeColor( true );
  SetAutoGo( !(flags & MANUAL) );
  if (GameCnt >= 0)
    UnmakeMove (board.side, &Game[GameCnt].move);
  else
    printf (_("No moves to undo!\n"));
  MoveLimit[board.side]++;
  TimeLimit[board.side] += Game[GameCnt+1].et;
  if (!(flags & XBOARD)) ShowBoard ();
}

void cmd_usage(void)
{
      printf ( "\n" );
      printf ( _("\
Usage: %s [OPTION]...\n\n"), progname );
      fputs( _("\
Play the game of chess\n\n"), stdout );
      fputs( _("Options:\n"), stdout );
      fputs( _("\
 -h, --help         display this help and exit\n"), stdout );
      fputs( _("\
 -v, --version      display version information and exit\n"), stdout );
      fputs( _("\
 -q, --quiet        make the program silent on startup\n"), stdout );
      fputs( _("\
     --silent       same as -q\n"), stdout );
      fputs( _("\
\n"), stdout );
      fputs( _("\
 -x, --xboard       start in engine mode\n"), stdout );
      fputs( _("\
 -p, --post         start up showing thinking\n"), stdout );
      fputs( _("\
 -e, --easy         disable thinking in opponents time\n"), stdout );
      fputs( _("\
 -m, --manual       enable manual mode\n"), stdout );
      fputs( _("\
 -u, --uci          enable UCI protocol (externally behave as UCI engine)\n"), stdout );
      fputs( _("\
 -M size, --memory=size   specify memory usage in MB for hashtable\n"), stdout );
      fputs( _("\
 -a filename, --addbook=filename   compile book.bin from pgn book 'filename'\n"), stdout );
      fputs( _("\
 -g, --graphic      enable graphic mode\n"), stdout );
      fputs( _("\
\n"), stdout );
      fputs( _("\
 Options xboard and post are accepted without leading dashes\n\
 for backward compatibility.\n\
\n"), stdout );
      fputs( _("\
 Moves are accepted either in standard algebraic notation (SAN) or\n\
 in coordinate algebraic notation.\n\
\n"), stdout );
      fputs( _("\
 The file 'gnuchess.ini' allows setting config options. See\n\
 'info gnuchess' for details. The file will be looked up in the current\n\
 directory or, if not found there, in the directory pointed to by\n\
 environment variable GNUCHESS_PKGDATADIR.\n\
\n"), stdout );
      fputs( _("\
Report bugs to <bug-gnu-chess@gnu.org>.\n\
\n"), stdout );
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
     pgnloaded = 0;
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
     printf(_("Invalid move: %s\n"),token[1]);
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
  printf( _("Command 'white' is currently not supported.\n") );
}

void cmd_xboard(void)
{
  SetDataToEngine( "xboard" );
  if (tokeneq (token[1], "off"))
    CLEAR (flags, XBOARD);
  else if (tokeneq (token[1], "on"))
    SET (flags, XBOARD);
  else if (!(flags & XBOARD)) { /* set if unset and only xboard called */
    SET (flags, XBOARD);        /* like in xboard/winboard usage */
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
      printf(_("My rating = %d\n"),myrating);
      printf(_("Opponent rating = %d\n"),opprating);
   }
   else if (tokeneq (token[1], _("time")))
      ShowTime ();
   else if (tokeneq (token[1], _("moves"))) {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenMoves (1);
      ShowMoveList (1);
      printf (_("No. of moves generated = %ld\n"), GenCnt);
   }
   else if (tokeneq (token[1], "escape")) {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenCheckEscapes (1);
      ShowMoveList (1);
      printf (_("No. of moves generated = %ld\n"), GenCnt);
   }
   else if (tokeneq (token[1], "noncapture"))
   {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenNonCaptures (1);
      FilterIllegalMoves (1);
      ShowMoveList (1);
      printf (_("No. of moves generated = %ld\n"), GenCnt);
   }
   else if (tokeneq (token[1], "capture"))
   {
      GenCnt = 0;
      TreePtr[2] = TreePtr[1];
      GenCaptures (1);
      FilterIllegalMoves (1);
      ShowMoveList (1);
      printf (_("No. of moves generated = %ld\n"), GenCnt);
   }
   else if (tokeneq (token[1], "eval") || tokeneq (token[1], "score"))
   {
      printf( _("Command 'show eval/score' is currently not supported.\n") );
      return;
   }
   else if (tokeneq (token[1], "game"))
     ShowGame ();
   else if (tokeneq (token[1], "pin"))
   {
      printf( _("Command 'show pin' is currently not supported.\n") );
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
  printf( _("Command 'test' is currently not supported.\n") );
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
   "quit",
   gettext_noop(" Quits the program."),
   "exit",
   gettext_noop(" In analysis mode this stops analysis, otherwise it quits the program."),
   "help",
   gettext_noop(" Produces a help blurb corresponding to this list of commands."),
   "book",
   gettext_noop(" add - compiles book.bin from a pgn book file"),
   gettext_noop(" on - enables use of book (default)"),
   gettext_noop(" off - disables use of book"),
   gettext_noop(" worst - plays worst move from book"),
   gettext_noop(" best - plays best move from book"),
   gettext_noop(" prefer - same as 'book on' (default)"),
   gettext_noop(" random - plays any move from book"),
   "version",
   gettext_noop(" Prints out the version of this program."),
   "previous",
   "p",
   gettext_noop(" Backs up one move in pgn loaded game."),
   "pgnsave FILENAME",
   gettext_noop(" Saves the game so far from memory to the file."),
   "pgnload FILENAME",
   gettext_noop(" Loads the game in the file into memory."),
   "pgnreplay FILENAME",
   gettext_noop(" Loads the game in the file into memory, and enables\n"
                " the commands 'first', 'last', 'next', 'previous'."),
   "next",
   "n",
   gettext_noop(" Advances one move in pgn loaded game."),
   "first",
   gettext_noop(" Goes to begin position of pgn loaded game."),
   "last",
   gettext_noop(" Goes to end position of pgn loaded game."),
   "force",
   "manual",
   gettext_noop(" Makes the program stop moving. You may now enter moves\n"
                " to reach some position in the future."),
   " ",
   "white",
   gettext_noop(" Program plays white."),
   "black",
   gettext_noop(" Program plays black."),
   "go",
   gettext_noop(" Computer takes whichever side is on move and begins its\n"
                " thinking immediately."),
   "post",
   gettext_noop(" Arranges for verbose thinking output showing variation, score,\n"
                " time, depth, etc."),
   "nopost",
   gettext_noop(" Turns off verbose thinking output."),
   "name NAME",
   gettext_noop(" Lets you input your name. Also writes the log.nnn and\n"
                " corresponding game.nnn files. For details please see\n"
                " the auxiliary file format sections."),
   "result",
   gettext_noop(" Mostly used by Internet Chess Server."),
   "activate",
   gettext_noop(" This command reactivates a game that has been terminated automatically\n"
                " due to checkmate or no more time on the clock. However, it does not\n"
                " alter those conditions. So you would have to undo a move or two, or\n"
                " add time to the clock with 'level' or 'time'."),
   "rating COMPUTERRATING OPPONENTRATING",
   gettext_noop(" Inputs the estimated rating for computer and for its opponent."),
   "new",
   gettext_noop(" Sets up a new game (i.e. pieces in original positions)."),
   "time",
   gettext_noop(" Inputs time left in game for computer in hundredths of a second.\n"
                " Mostly used by Internet Chess Server."),
   "hash",
   gettext_noop(" on - enables using the memory hash table to speed up search"),
   gettext_noop(" off - disables the memory hash table"),
   "memory N",
   gettext_noop(" Sets the hash table to permit storage of N MB."),
   "null",
   gettext_noop(" on - enables using the null-move heuristic to speed up search"),
   gettext_noop(" off - disables using the null-move heuristic"),
   "xboard",
   gettext_noop(" on - enables use of xboard/winboard"),
   gettext_noop(" off - disables use of xboard/winboard"),
   "depth N",
   gettext_noop(" Sets the program to look N ply (half-moves) deep for every\n"
                " search it performs. If there is a checkmate or other condition\n"
                " that does not allow that depth, then it will not be."),
   "level MOVES MINUTES INCREMENT",
   gettext_noop(" Sets time control to be MOVES in MINUTES, with each move giving\n"
                " an INCREMENT (in seconds, i.e. a Fischer-style clock)."),
   "load",
   "epdload",
   gettext_noop(" Loads a position in EPD format from disk into memory."),
   "save",
   "epdsave",
   gettext_noop(" Saves game position into EPD format from memory to disk."),
   "switch",
   gettext_noop(" Switches side to move."),
   "solve FILENAME",
   "solveepd FILENAME",
   gettext_noop(" Solves the positions in FILENAME."),
   "remove",
   gettext_noop(" Backs up two moves in game history."),
   "undo",
   gettext_noop(" Backs up one move in game history."),
   "usage",
   gettext_noop(" Displays command line syntax."),
   "show",
   gettext_noop(" board - displays the current board"),
   gettext_noop(" time - displays the time settings"),
   gettext_noop(" moves - shows all moves using one call to routine"),
   gettext_noop(" escape - shows moves that escape from check using one call to routine"),
   gettext_noop(" noncapture - shows non-capture moves"),
   gettext_noop(" capture - shows capture moves"),
   gettext_noop(" eval [or score] - shows the evaluation per piece and overall"),
   gettext_noop(" game - shows moves in game history"),
   gettext_noop(" pin - shows pinned pieces"),
   "test",
   gettext_noop(" movelist - reads in an epd file and shows legal moves for its entries"),
   gettext_noop(" capture - reads in an epd file and shows legal captures for its entries"),
   gettext_noop(" movegenspeed - tests speed of move generator"),
   gettext_noop(" capturespeed - tests speed of capture move generator"),
   gettext_noop(" eval - reads in an epd file and shows evaluation for its entries"),
   gettext_noop(" evalspeed - tests speed of the evaluator"),
   "bk",
   gettext_noop(" Shows moves from opening book."),
   "graphic",
   gettext_noop(" Enables display board in graphic mode."),
   "nographic",
   gettext_noop(" Disables graphic mode and display classical view."),
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
        if (strncmp(*p, token[1], strlen(token[1])) == 0) {
           puts(*p);
           while (*++p && **p != ' ') /* Skip aliases */ ;
           for (; *p && **p == ' '; p++) {
              puts(_(*p));
           }
           return;
        }
      }
      printf(_("Help for command %s not found\n\n"), token[1]);
   }
   printf(_("List of commands: (help COMMAND to get more help)\n"));
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

/* Last entry contains two NULL pointers */

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
  { "graphic", cmd_graphic },
  { "hard", cmd_hard },
  { "hash", cmd_hash },
  { "help", cmd_help },
  { "hint", cmd_hint },
  { "ics", cmd_ics },
  { "last", cmd_last },
  { "level", cmd_level },
  { "list", cmd_list },
  { "load", cmd_load },
  { "manual", cmd_manual },
  { "memory", cmd_memory },
  { "name", cmd_name },
  { "new", cmd_new },
  { "next", cmd_next },
  { "n", cmd_next },
  { "nographic", cmd_nographic  },
  { "nopost", cmd_nopost },
  { "null", cmd_null },
  { "otim", cmd_otim },
  { "pgnload", cmd_pgnload },
  { "pgnreplay", cmd_pgnreplay },
  { "pgnsave", cmd_pgnsave },
  { "ping", cmd_ping },
  { "post", cmd_post },
  { "previous", cmd_previous },
  { "p", cmd_previous },
  { "first", cmd_first },
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
     pgnloaded = 0;
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
     printf(_("Invalid move: %s\n"),token[0]);
     fflush(stdout);
   }
}

void check_board()
/*************************************************************************
 *
 *  When the board is changed by commands, call the Validation
 *  routine, and if it fails set flags to prevent the analysis of
 *  illegal positions, as the code is not robust against the
 *  analysis of such positions (To Do).
 *
 *************************************************************************/
{
  if (!ValidateBoard()) {
    SET (flags, ENDED);
    if (flags & XBOARD) {
    printf ("telluser Board is wrong!\n");
        fflush(stdout);
    } else {
        printf (_("Board is wrong!\n"));
    }
  }
}

