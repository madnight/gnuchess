/* pgn.cc

   GNU Chess frontend

   Copyright (C) 2001-2017 Free Software Foundation, Inc.

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
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#include "common.h"
#include "version.h"
#include "lexpgn.h"
#include "gettext.h"

#define _(str) gettext (str)

#define NULL2EMPTY(x) ( (x) ? (x) : "")

extern FILE *yyin;

extern int yylex (void);

void PGNSaveToFile (const char *file, const char *resultstr)
/****************************************************************************
 *
 *  To save a game into PGN format to a file.  If the file does not exist,
 *  it will create it.  If the file exists, the user will be asked for
 *  permission to overwrite the existing one.
 *
 ****************************************************************************/
{
   FILE *fp;
   char s[100];
   int len;
   char *p;
   int i;
   char answer = '\0'; /* Default: invalid */
   time_t secs;
   struct tm *timestruct;
   /* TRANSLATORS: "y" for "yes" */
   const char *yes = _("y");
   /* TRANSLATORS: "n" for "no" */
   const char *no = _("n");

   fp = fopen(file, "r");
   if (fp) {
     fclose(fp);
     printf(_("File with name %s already exists.\n"), file);
     do {
       /* TRANSLATORS: translate "y/n", for "yes/no". */
       printf(_("Overwrite file? [y/n]: "));
       answer = getchar();
       
       if ( answer == '\n' ) {
         printf(_("Invalid answer! "));         
         continue;
       }
       
       while( getchar() != '\n' );
       
       if (tolower(answer) == *no) {
         printf(_("File not saved.\n"));
         return;
       }
       else if (tolower(answer) == *yes) {
         printf(_("File %s is overwritten.\n"), file);
       }
       else {
         printf(_("Invalid answer! "));
       } 
     } while (tolower(answer) != *no && tolower(answer) != *yes);
   }
   
   fp = fopen (file, "w");
   if (fp == NULL)
   {
      printf ("Cannot write to file %s\n", file);
      return;
   }

   /* Write the seven tags */
   fprintf (fp, "[Event \"%s\"]\n", NULL2EMPTY(pgn_event));
   fprintf (fp, "[Site \"%s\"]\n", NULL2EMPTY(pgn_site));
   secs=time(0);
   if (pgn_date)
     fprintf(fp,"[Date \"%s\"]\n", pgn_date);
   else {
     timestruct=localtime((time_t*) &secs);
     fprintf(fp,"[Date \"%4d.%02d.%02d\"]\n",timestruct->tm_year+1900,
             timestruct->tm_mon+1,timestruct->tm_mday);
   }
   fprintf (fp, "[Round \"%s\"]\n", NULL2EMPTY(pgn_round));

   if (pgn_white)
     fprintf (fp, "[White \"%s\"]\n", pgn_white);
   else if (computer == white)
     fprintf (fp, "[White \"%s %s\"]\n",PROGRAM,VERSION);
   else
     fprintf (fp, "[White \"%s\"]\n",name);

   if (pgn_black)
     fprintf (fp, "[Black \"%s\"]\n", pgn_black);
   else if (computer == black)
     fprintf (fp, "[Black \"%s %s\"]\n",PROGRAM,VERSION);
   else
     fprintf (fp, "[Black \"%s\"]\n",name);

   if (pgn_whiteELO)
     fprintf (fp, "[WhiteELO \"%s\"]\n", NULL2EMPTY(pgn_white));
   else
     fprintf(fp, "[WhiteELO \"%d\"]\n",computer==white?myrating:opprating);
   if (pgn_blackELO)
     fprintf (fp, "[BlackELO \"%s\"]\n", NULL2EMPTY(pgn_black));
   else
     fprintf (fp, "[BlackELO \"%d\"]\n",computer==white?opprating:myrating);

   if (pgn_result)
     fprintf (fp, "[Result \"%s\"]\n", pgn_result);
   else {
     /* Revive the little-known standard functions! */
     len = strcspn(resultstr," {");
     fprintf (fp, "[Result \"%.*s\"]\n", len, resultstr);
   }

   if (pgn_othertags) {
     fprintf (fp, "%s", pgn_othertags);
   }

   fprintf (fp, "\n");

   if (initial_comments)
   {
     fprintf(fp, "\n%s\n", initial_comments);
     /* If it doesn't end in \n, add it */
     if (initial_comments[0] &&
         initial_comments[strlen(initial_comments)-1] != '\n')
        fprintf(fp, "\n");
   }

   s[0] = '\0';
   for (i = 0; i <= GameCnt; i++)
   {
      if (! (i % 2)) {
        sprintf (s, "%s%d. ", s, i/2 + 1);
      }
      sprintf (s, "%s%s ", s, Game[i].SANmv);
      if (strlen (s) > 80)
      {
         p = s + 79;
         while (*p-- != ' ');
         *++p = '\0';
         fprintf (fp, "%s\n", s);
         strcpy (s, p+1);
      }
      if (Game[i].comments) {
         fprintf (fp, "%s\n", s);
         fprintf (fp, "%s", Game[i].comments);
         if (Game[i].comments[0] &&
             Game[i].comments[strlen(Game[i].comments)-1] != '\n')
              fprintf(fp, "\n");
         s[0] = '\0';
      }
   }
   fprintf (fp, "%s", s);
   fprintf (fp, "%s", resultstr);
   fprintf (fp, "\n\n");
   fclose (fp);

}


void PGNReadFromFile (const char *file, int showheading)
/****************************************************************************
 *
 *  To read a game from a PGN file.
 *
 ****************************************************************************/
{
   FILE *fp;

   fp = fopen (file, "r");
   if (fp == NULL)
   {
      printf ("Cannot open file %s\n", file);
      return;
   }
   yyin = fp;

   InitVars ();

   data_dest = DEST_GAME;
   (void) yylex();

   fclose (fp);

   ShowBoard ();

   if ( showheading ) {
      printf("\n--------------------------------------------------\n");
      printf("%s (%s) x %s (%s) - %s\nSite: %s\nDate: %s\n",
             pgn_white  != NULL ? pgn_white  : "Unknown", pgn_whiteELO != NULL ? pgn_whiteELO : "Unknown",
             pgn_black  != NULL ? pgn_black  : "Unknown", pgn_blackELO != NULL ? pgn_blackELO : "Unknown",
             pgn_result != NULL ? pgn_result : "Unknown", pgn_site     != NULL ? pgn_site     : "Unknown",
             pgn_date   != NULL ? pgn_date   : "Unknown");
      printf("--------------------------------------------------\n");
   }
}

/* Only players in the table below are permitted into the opening book
   from the PGN files. Please expand the table as needed. Generally,
   I would recommend only acknowledged GM's and IM's and oneself, but
   because of the self-changing nature of the book, anything inferior
   will eventually be eliminated through automatic play as long as
   you feed the games the program plays back to itself with "book add pgnfile"
*/
/* TODO: Fix this so the list isn't hardcoded. */

static const char *const trusted_players[] = {
  "Alekhine",
  "Adams",
  "Anand",
  "Anderssen",
  "Andersson",
  "Aronin",
  "Averbakh",
  "Balashov",
  "Beliavsky",
  "Benko",
  "Bernstein",
  "Bird",
  "Bogoljubow",
  "Bolbochan",
  "Boleslavsky",
  "Byrne",
  "Botvinnik",
  "Bronstein",
  "Browne",
  "Capablanca",
  "Chigorin",
  "Christiansen",
  "De Firmian",
  "Deep Blue",
  "Deep Thought",
  "Donner",
  "Dreev",
  "Duras",
  "Euwe",
  "Evans",
  "Filip",
  "Fine",
  "Fischer",
  "Flohr",
  "Furman",
  "Gelfand",
  "Geller",
  "Gereben",
  "Glek",
  "Gligoric",
  "GNU",
  "Golombek",
  "Gruenfeld",
  "Guimard",
  "Hodgson",
  "Ivanchuk",
  "Ivkov",
  "Janowsky",
  "Kamsky",
  "Kan",
  "Karpov",
  "Kasparov",
  "Keres",
  "Korchnoi",
  "Kortschnoj",
  "Kotov",
  "Kramnik",
  "Kupreich",
  "Lasker",
  "Lautier",
  "Letelier",
  "Lilienthal",
  "Ljubojevic",
  "Marshall",
  "Maroczy",
  "Mieses",
  "Miles",
  "Morphy",
  "Mueller",     /* Every other German has this name... */
  "Nimzowitsch",
  "Nunn",
  "Opocensky",
  "Pachman",
  "Petrosian",
  "Piket",
  "Pilnik",
  "Pirc",
  "Polgar",
  "Portisch",
  "Psakhis",
  "Ragozin",
  "Reshevsky",
  "Reti",
  "Romanish",
  "Rubinstein",
  "Saemisch",
  "Seirawan",
  "Shirov",
  "Short",
  "Silman",
  "Smyslov",
  "Sokolsky",
  "Spassky",
  "Sveshnikov",
  "Stahlberg",
  "Steinitz",
  "Tal",
  "Tarjan",
  "Tartakower",
  "Timman",
  "Topalov",
  "Torre",
  "Vidmar"

  };

int IsTrustedPlayer(const char *name)
/* Return 1 if name in trusted_players list, else 0 */
{
	int i;
	for (i = 0; i < (sizeof(trusted_players) / sizeof(*trusted_players));
	    	i++) {
		if (strstr(name, trusted_players[i]) != NULL)
			return 1;
       }
	return 0;
}
