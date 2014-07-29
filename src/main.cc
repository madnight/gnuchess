/* GNU Chess 6 - main.cc - entry point

   Copyright (c) 2001-2014 Free Software Foundation, Inc.

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

#include "components.h"

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h> 
#include <unistd.h>
#include <locale.h>

#include "frontend/common.h"
#include "gettext.h"

unsigned char lzArray[65536];
BitBoard BitPosArray[64];
BitBoard NotBitPosArray[64];
BitBoard MoveArray[8][64];
BitBoard Ray[64][8];
BitBoard FromToRay[64][64];
BitBoard RankBit[8];
BitBoard FileBit[8];
BitBoard Ataks[2][7];
BitBoard Rook00Atak[64][256];
BitBoard Rook90Atak[64][256];
BitBoard Bishop45Atak[64][256];
BitBoard Bishop315Atak[64][256];
short directions[64][64];
unsigned char BitCount[65536];
leaf Tree[MAXTREEDEPTH];
leaf *TreePtr[MAXPLYDEPTH];
GameRec Game[MAXGAMEDEPTH];
int GameCnt;
int RealGameCnt;
short RealSide;
int computer;
unsigned int flags;
unsigned int preanalyze_flags;
int cboard[64];
int Mvboard[64];
Board board;
HashType hashcode[2][7][64];
HashType ephash[64];
HashType WKCastlehash;
HashType WQCastlehash;
HashType BKCastlehash;
HashType BQCastlehash;
HashType Sidehash;
HashType HashKey;
HashType PawnHashKey;
int Game50;
unsigned long GenCnt;
char SANmv[SANSZ];
char id[32];
char solution[64];
float SearchTime;
int MoveLimit[2];
float TimeLimit[2];
int TCMove;
int TCinc;
float TCTime;
int phase;
short bookfirstlast;
short graphicmodeoutput;
short pgnloaded = 0;
int pgncnt = 0;
    
char *progname;
FILE *ofp;
int myrating, opprating;
char name[50];
int computerplays;		/* Side computer is playing */
int n;		/* Last mobility returned by CTL */
int ExchCnt[2];	/* How many exchanges? */

int slider[8] = { 0, 0, 0, 1, 1, 1, 0, 0 };
int Value[7] = { 0, ValueP, ValueN, ValueB, ValueR, ValueQ, ValueK};
int range[8] = { 0, 0, 0, 1, 1, 1, 0, 0 };
int ptype[2] = { pawn, bpawn };
char algbr[64][3] =
{ "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" 
};
char algbrfile[9] = "abcdefgh";
char algbrrank[9] = "12345678";
  
char notation[8] = { " PNBRQK" };
char lnotation[8] = { " pnbrqk" };

short Shift00[64] =
{ 56, 56, 56, 56, 56, 56, 56, 56,
  48, 48, 48, 48, 48, 48, 48, 48,
  40, 40, 40, 40, 40, 40, 40, 40,
  32, 32, 32, 32, 32, 32, 32, 32,
  24, 24, 24, 24, 24, 24, 24, 24,
  16, 16, 16, 16, 16, 16, 16, 16,
   8,  8,  8,  8,  8,  8,  8,  8,
   0,  0,  0,  0,  0,  0,  0,  0
};

int r90[64] =
{ A8, A7, A6, A5, A4, A3, A2, A1,
  B8, B7, B6, B5, B4, B3, B2, B1,
  C8, C7, C6, C5, C4, C3, C2, C1,
  D8, D7, D6, D5, D4, D3, D2, D1,
  E8, E7, E6, E5, E4, E3, E2, E1,
  F8, F7, F6, F5, F4, F3, F2, F1,
  G8, G7, G6, G5, G4, G3, G2, G1,
  H8, H7, H6, H5, H4, H3, H2, H1 };

short Shift90[64] =
{ 0, 8, 16, 24, 32, 40, 48, 56,
  0, 8, 16, 24, 32, 40, 48, 56,
  0, 8, 16, 24, 32, 40, 48, 56,
  0, 8, 16, 24, 32, 40, 48, 56,
  0, 8, 16, 24, 32, 40, 48, 56,
  0, 8, 16, 24, 32, 40, 48, 56,
  0, 8, 16, 24, 32, 40, 48, 56,
  0, 8, 16, 24, 32, 40, 48, 56
};

int r45[64] =
{ E4, F3, H2, C2, G1, D1, B1, A1,
  E5, F4, G3, A3, D2, H1, E1, C1,
  D6, F5, G4, H3, B3, E2, A2, F1, 
  B7, E6, G5, H4, A4, C3, F2, B2,
  G7, C7, F6, H5, A5, B4, D3, G2, 
  C8, H7, D7, G6, A6, B5, C4, E3, 
  F8, D8, A8, E7, H6, B6, C5, D4, 
  H8, G8, E8, B8, F7, A7, C6, D5 };

short Shift45[64] =
{ 28, 36, 43, 49, 54, 58, 61, 63,
  21, 28, 36, 43, 49, 54, 58, 61,
  15, 21, 28, 36, 43, 49, 54, 58,
  10, 15, 21, 28, 36, 43, 49, 54,
   6, 10, 15, 21, 28, 36, 43, 49,
   3,  6, 10, 15, 21, 28, 36, 43,
   1,  3,  6, 10, 15, 21, 28, 36,
   0,  1,  3,  6, 10, 15, 21, 28 };

int Mask45[64] =
{ 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01,
  0x7F, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 
  0x3F, 0x7F, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 
  0x1F, 0x3F, 0x7F, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 
  0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x7F, 0x3F, 0x1F, 
  0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x7F, 0x3F, 
  0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x7F, 
  0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };

int r315[64] =
{ A1, C1, F1, B2, G2, E3, D4, D5,
  B1, E1, A2, F2, D3, C4, C5, C6,
  D1, H1, E2, C3, B4, B5, B6, A7,
  G1, D2, B3, A4, A5, A6, H6, F7,
  C2, A3, H3, H4, H5, G6, E7, B8,
  H2, G3, G4, G5, F6, D7, A8, E8,
  F3, F4, F5, E6, C7, H7, D8, G8,
  E4, E5, D6, B7, G7, C8, F8, H8 };

short Shift315[64] =
{ 63, 61, 58, 54, 49, 43, 36, 28,
  61, 58, 54, 49, 43, 36, 28, 21,
  58, 54, 49, 43, 36, 28, 21, 15,
  54, 49, 43, 36, 28, 21, 15, 10,
  49, 43, 36, 28, 21, 15, 10,  6,
  43, 36, 28, 21, 15, 10,  6,  3,
  36, 28, 21, 15, 10,  6,  3,  1,
  28, 21, 15, 10,  6,  3,  1,  0 };

int Mask315[64] =
{ 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF,
  0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x7F,
  0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x7F, 0x3F,
  0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0x7F, 0x3F, 0x1F,
  0x1F, 0x3F, 0x7F, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F,
  0x3F, 0x7F, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07,
  0x7F, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03,
  0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };

extern char userinputbuf[];

int main (int argc, char *argv[])
{
  int i;

  /* Set locale via LC_ALL.  */
  setlocale (LC_ALL, "");

#if ENABLE_NLS
  /* Set the text message domain.  */
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif

  /*
   * Parse command line arguments conforming with getopt_long syntax
   * Note: we have to support "xboard" and "post" as bare strings
   * for backward compatibility.
   */
 
  int c;
  int opt_help = 0, opt_version = 0, opt_post = 0, opt_xboard = 0, opt_memory = 0,
      opt_easy = 0, opt_manual = 0, opt_quiet = 0, opt_uci = 0, opt_graphic = 0;
  char opt_addbook[MAXSTR+1] = "";
  char *endptr;

  /*disable graphic output by default */
  graphicmodeoutput = 0;
  

  progname = argv[0]; /* Save in global for cmd_usage */

  while (1)
  {
    static struct option long_options[] =
    {
        {"memory", 1, 0, 'M'},
        {"version", 0, 0, 'v'},
        {"quiet", 0, 0, 'q'},
        {"silent", 0, 0, 'q'},
        {"help", 0, 0, 'h'},
        {"xboard", 0, 0, 'x'},
        {"post", 0, 0, 'p'},
        {"easy", 0, 0, 'e'},
        {"manual", 0, 0, 'm'},
        {"uci", 0, 0, 'u'},
        {"addbook", 1, 0, 'a'},
        {"graphic", 0, 0, 'g'},                
        {0, 0, 0, 0}
    };
 
    /* getopt_long stores the option index here. */ 

    int option_index = 0;
 
    c = getopt_long (argc, argv, "qehmpvxgM:ua:",
             long_options, &option_index);
 
    /* Detect the end of the options. */
   if (c == -1)
     break;

   /* 
    * Options with a straight flag, could use getoopt_long
    * flag setting but this is more "obvious" and easier to
    * modify.
    */
   switch (c)
     {
     case 'v':
       opt_version = 1;
       break;
     case 'h':
       opt_help = 1;
       break;
     case 'q':
       opt_quiet = 1;
       break;
     case 'x':
       opt_xboard = 1;
       break;
     case 'p':
       opt_post = 1;
       break;
     case 'e':
       opt_easy = 1;
       break;
     case 'g':
         opt_graphic = 1;
       break;
     case 'm':
       opt_manual = 1;
       break;       
     case 'M':    
       if  ( optarg == NULL ){ /* we have error such as two -M */
         opt_help = 1;
         break;
       }
       errno = 0; /* zero error indicator */
       opt_memory = strtol (optarg, &endptr, 10);
       if ( errno != 0 || *endptr != '\0' ){
         printf("Memory out of Range or Invalid\n");
         return(1);
       }
       break;
     case '?': /* On error give help - getopt does a basic message. */
       opt_help = 1;
       break;
     case 'u':
       opt_uci = 1;
       break;
     case 'a':    
       if  ( optarg == NULL ){ /* we have error such as two -a */
         opt_help = 1;
         break;
       }
       errno = 0; /* zero error indicator */
       if ( strlen( optarg ) > MAXSTR ) {
         printf( "File name is too long (max = %d)\n", MAXSTR );
         return(1);
       }
       strcpy( opt_addbook, optarg );
       break;
     default:
       puts ("Option Processing Failed\n");
       abort();
     }
  } /* end of getopt_long style parsing */

  /* Initialize random number generator */
  srand((unsigned int) time(NULL));
  
  /* initialize control flags */
  flags = ULL(0);

  /* output for thinking */
  ofp = stdout;

  /* Handle old style command line options */
  if (argc > 1) {
    for (i = 0; i < argc; i++) {
      if (strcmp(argv[i],"xboard") == 0) {
	SET (flags, XBOARD);
      } else if (strcmp(argv[i],"post") == 0) {
	SET (flags, POST);
      }
    }
  }
  if (opt_xboard == 1)
    SET (flags, XBOARD);
  if (opt_uci == 1)
    SET (flags, UCI);
  if (opt_post == 1) {
    SET (flags, POST);	
  }
  if (opt_manual ==1)
    SET (flags, MANUAL);
  cmd_version();
  
  /* If the version option was specified we can exit here */
  if (opt_version == 1)
	return(0);
  
  /* Startup output */
  if ( !( flags & XBOARD ) && ( !opt_quiet ) && ( !opt_uci) ) {
    printf( "Copyright (C) 2014 Free Software Foundation, Inc.\n" );
    printf( "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n" );
    printf( "This is free software: you are free to change and redistribute it.\n" );
    printf( "There is NO WARRANTY, to the extent permitted by law.\n" );
  }

  /* If a usage statement is required output it here */
  if (opt_help == 1){
    cmd_usage();
    return (1); /* Maybe an error if due to bad arguments. */
  }

  dbg_open(NULL);

  if ( opt_memory != 0 )
    ; /* TODO: to be removed - this is handled by the adapter */

  Initialize ();
  InitFrontend();
  if ( ! (flags & UCI ) ) {
    /* Initialize the adapter, which in turn initializes the engine */
    InitAdapter();
  } else {
    /* Initialize the engine only; no adapter */
    InitEngine();
  }

  /* Compile book if the addbook option was specified. Ignore any other options. */
  if ( strlen( opt_addbook ) > 0 ) {
    char data[9+MAXSTR+1+4]="";
    sprintf( data, "book add %s\nquit", opt_addbook );
    SendToEngine( data );
  }

  if ( opt_easy == 0 )
    SET (flags, HARD);
  else {
    char data[9];
    strcpy( data, "easy" );
    SendToEngine( data );
  }

  if ( opt_memory > 0 ) {
    char data[20];
    sprintf( data, "memory %d", opt_memory );
    SendToEngine( data );
  }

  if (opt_post == 1) {
    SET (flags, POST);	
    char data[9];
    strcpy( data, "post" );
    SendToEngine( data );
  }

  if (opt_manual == 1) {
    SET (flags, MANUAL);
    char data[9];
    strcpy( data, "force" );
    SendToEngine( data );
  }

  if (argc > 1) {
    for (i = 0; i < argc; i++) {
      if (strcmp(argv[i],"xboard") == 0) {
	SET (flags, XBOARD);
      } else if (strcmp(argv[i],"post") == 0) {
	SET (flags, POST);
      } 
    }

  if ( opt_graphic == 1) {
    graphicmodeoutput = 1;
  }
  
  }

  bookfirstlast = 3;

  
  /* While not quit
   *     If user input buffer not empty
   *         Read next line from user input buffer
   *         Process user input at front-end level
   *         If user input must be passed to engine
   *             Send user input to engine
   *     If engine input buffer not empty
   *         Read next line from engine input buffer
   *         Process engine input at front-end level
   *             Send user input to user (print on term)
   *         If engine input must be passed to user
   *     If user input is ready for reading (select)
   *         Add user input to user input buffer
   *     If engine input is ready for reading (select)
   *         Add engine input to engine input buffer
   */

  //usleep(3000); /* So that Polyglot's and Fruit's banner has enough time to be displayed */
  //strcpy( userinputbuf, "protover 2\n" );
  while (!(flags & QUIT)) {
    if ( flags & UCI ) {
      /* In UCI mode, just forward input/output to/from engine - no adapter */
      ForwardUserInputToEngine();
      ForwardEngineOutputToUser();
    } else { /* Classical GNU Chess mode */
      /* Prompt the user, if neither in xboard nor uci mode */
      ShowPrompt();
      /* Check if there is a new command from the user */
      NextUserCmd();
      /* Show thinking message */
      if ((flags & THINK) && !(flags & MANUAL) && !(flags & ENDED)) {
        if (!(flags & XBOARD)) printf("Thinking...\n");
        CLEAR (flags, THINK);
      }
      /* Check if there is a new command from the engine */
      NextEngineCmd();
      /* Check if user input ready for reading. If so, store it in a buffer. */
      ReadFromUser();
      /* Check if engine input ready for reading. If so, store it in a buffer. */
      ReadFromEngine();
    }
    /* Avoid using 100% CPU */
    usleep(100);
  }
  
  dbg_close();
  return (0);
}
