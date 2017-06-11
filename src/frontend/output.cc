/* output.c

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
#include <string.h>
#include "common.h"

#define MAX_BOARD_RANGE 65
#define PIECE_SIZE 8
const char w_pieces[6][PIECE_SIZE] = {"\u2654 ", "\u2655 ", "\u2656 ", "\u2657 ", "\u2658 ", "\u2659 "};
const char b_pieces[6][PIECE_SIZE] = {"\u265A ", "\u265B ", "\u265C ", "\u265D ", "\u265E ", "\u265F "};
const unsigned ui_king = 0;
const unsigned ui_queen = 1;
const unsigned ui_rook = 2;
const unsigned ui_bishop = 3;
const unsigned ui_knight = 4;
const unsigned ui_pawn = 5;

const char white_square[] = "\033[7;37m";
const char black_square[] = "\033[7;35m";
const char default_console[] = "\033[0m";
const char blank_place[] = "  ";


/**************************************************************************
 * Print out the board in a new style way.
 **************************************************************************/
static void ShowStylishBoard(const char *boardmap);

/**************************************************************************
 * Fill the array piece with the new style piece code
 **************************************************************************/
static void GetPiece(const char orig_piece, char *piece);

/**************************************************************************
 * Print out the classical gnuchess board.
 **************************************************************************/
static void ShowClassicalBoard(const char *boardmap);


void ShowTime (void)
/**************************************************************************
 *
 *  Print out the time settings.
 *
 **************************************************************************/
{
}

void ShowMoveList (int ply)
/**************************************************************************
 *
 *  Print out the move list.
 *
 **************************************************************************/
{
   leaf *node;
   int i = 0;

   for (node = TreePtr[ply]; node < TreePtr[ply+1]; node++)
   {
      SANMove (node->move, ply);
      printf ("%5s %3d\t", SANmv, SwapOff(node->move));
      if (++i == 5)
      {
         printf ("\n");
         i = 0;
      }
   }
   printf ("\n");
}


void ShowBoard (void)
/*****************************************************************************
 *
 *  Display the board.  Not only that but display some useful information
 *  like whether enpassant is legal and castling state.
 *
 *****************************************************************************/
{
   int r, c, sq;

   fprintf (ofp, "\n");

   if (coords == 1) {
      fprintf(ofp, "  ");
   }

   if (board.side == white)
      fprintf (ofp, "white  ");
   else
      fprintf (ofp, "black  ");

   if (board.flag & WKINGCASTLE)
      fprintf (ofp, "K");
   if (board.flag & WQUEENCASTLE)
      fprintf (ofp, "Q");
   if (board.flag & BKINGCASTLE)
      fprintf (ofp, "k");
   if (board.flag & BQUEENCASTLE)
      fprintf (ofp, "q");

   if (board.ep > -1)
      fprintf (ofp, "  %s", algbr[board.ep]);

   fprintf (ofp, "\n");

   char arr_board[MAX_BOARD_RANGE];
   unsigned bIndex = 0;
   memset(arr_board, '\0', MAX_BOARD_RANGE);

   for (r = 56; r >= 0; r -= 8)
   {
      for (c = 0; c < 8; c++)
      {
         sq = r + c;
         if (board.b[white][pawn]   & BitPosArray[sq])
            arr_board[bIndex++] = 'P';
         else if (board.b[white][knight] & BitPosArray[sq])
            arr_board[bIndex++] = 'N';
         else if (board.b[white][bishop] & BitPosArray[sq])
            arr_board[bIndex++] = 'B';
         else if (board.b[white][rook]   & BitPosArray[sq])
            arr_board[bIndex++] = 'R';
         else if (board.b[white][queen]  & BitPosArray[sq])
            arr_board[bIndex++] = 'Q';
         else if (board.b[white][king]   & BitPosArray[sq])
            arr_board[bIndex++] = 'K';
         else if (board.b[black][pawn]   & BitPosArray[sq])
            arr_board[bIndex++] = 'p';
         else if (board.b[black][knight] & BitPosArray[sq])
            arr_board[bIndex++] = 'n';
         else if (board.b[black][bishop] & BitPosArray[sq])
            arr_board[bIndex++] = 'b';
         else if (board.b[black][rook]   & BitPosArray[sq])
            arr_board[bIndex++] = 'r';
         else if (board.b[black][queen]  & BitPosArray[sq])
            arr_board[bIndex++] = 'q';
         else if (board.b[black][king]   & BitPosArray[sq])
            arr_board[bIndex++] = 'k';
         else
            arr_board[bIndex++] = '.';
      }
   }

   if ( graphicmodeoutput == 1) {
     ShowStylishBoard(arr_board);
   } else {
     ShowClassicalBoard(arr_board);
   }

  fprintf (ofp, "\n");
}


void ShowStylishBoard(const char *boardmap)
{
  unsigned i = 0;
  unsigned row = 8;
  const char column[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  int b_white = 1;
  char tmp_piece[PIECE_SIZE];
  memset(tmp_piece, '\0', PIECE_SIZE);

  if ( coords == 1 ) fprintf(ofp, "%d ", row);
  row--;

  for (i =0; i < MAX_BOARD_RANGE; ++i)
    {
        fprintf(ofp, "%s", default_console);

        if ((i > 0) && (i % 8 == 0))
            {
            fprintf(ofp, "\n");
            b_white = !b_white;

            if (row >=1) {
              if( coords == 1 ) fprintf(ofp, "%d ", row);
              row--;
            }
            else
                break;
            }

        GetPiece(boardmap[i], tmp_piece);

        if (b_white)
            {
            fprintf(ofp, "%s%s", white_square, tmp_piece);
            }
        else
            {
            fprintf(ofp, "%s%s", black_square, tmp_piece);
            }

        b_white = !b_white;
   }
   fprintf(ofp, "%s  ", default_console);

   if ( coords == 1 ) {
     for (i = 0; i < 8; ++i) {
          fprintf(ofp, "%c ", column[i]);
     }
   }
}


void GetPiece(const char orig_piece, char *piece)
{
    if (orig_piece == 'p')
        strcpy(piece, b_pieces[ui_pawn]);
    else if (orig_piece == 'P')
        strcpy(piece, w_pieces[ui_pawn]);
    else if (orig_piece == 'n')
        strcpy(piece, b_pieces[ui_knight]);
    else if (orig_piece == 'N')
        strcpy(piece, w_pieces[ui_knight]);
    else if (orig_piece == 'b')
        strcpy(piece, b_pieces[ui_bishop]);
    else if (orig_piece == 'B')
        strcpy(piece, w_pieces[ui_bishop]);
    else if (orig_piece == 'r')
        strcpy(piece, b_pieces[ui_rook]);
    else if (orig_piece == 'R')
        strcpy(piece, w_pieces[ui_rook]);
    else if (orig_piece == 'q')
        strcpy(piece, b_pieces[ui_queen]);
    else if (orig_piece == 'Q')
        strcpy(piece, w_pieces[ui_queen]);
    else if (orig_piece == 'k')
        strcpy(piece, b_pieces[ui_king]);
    else if (orig_piece == 'K')
        strcpy(piece, w_pieces[ui_king]);
    else if (orig_piece == '.')
        strcpy(piece, blank_place);
}


void ShowClassicalBoard(const char *boardmap)
{
  unsigned i = 0;
  const char column[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  unsigned row = 8;
  
  for (i =0; i < MAX_BOARD_RANGE; ++i) {
    if ((i > 0) && (i % 8 == 0)) {
      fprintf(ofp, "\n");
    
      if ( coords == 1 ) {
        if (row >=1)
          fprintf(ofp, "%d ", row--);
        else
          break;
      }
    
    }

    fprintf(ofp, "%c ", boardmap[i]);
  }
  
  if ( coords == 1 ) {
    fprintf(ofp, "  ");
    for (i = 0; i < 8; ++i) {
      fprintf(ofp, "%c ", column[i]);
    }
  }
}


void ShowCBoard (void)
/*****************************************************************************
 *
 *
 *
 *****************************************************************************/
{
   int r, c;

   for (r = 56; r >= 0; r -= 8)
   {
      for (c = 0; c < 8; c++)
      {
         printf ("%2c ", cboard[r + c] ? notation[cboard[r+c]] : '.');
      }
      printf ("\n");
   }
   printf ("\n");
}


void ShowMvboard (void)
/*****************************************************************************
 *
 *  Print the Mvboard[] array.
 *
 *****************************************************************************/
{
   int r, c;

   for (r = 56; r >= 0; r -= 8)
   {
      for (c = 0; c < 8; c++)
      {
         printf ("%2d ", Mvboard[r + c]);
      }
      printf ("\n");
   }
   printf ("\n");
}

void ShowGame (void)
{
  int i;

/* *********************************************
   * We must handle the special case of an EPD *
   * game where the first move is by black     *
   ********************************************* */

  if ( GameCnt >= 0 )
  {

    printf ("      White   Black\n");

    if ( ( board.side == white && GameCnt % 2 == 1 ) ||
         ( board.side == black && GameCnt % 2 == 0 ))
    {

      for (i = 0; i <= GameCnt; i += 2)
        {
          printf ("%3d.  %-7s %-7s\n", i/2 + 1, Game[i].SANmv,
    	      Game[i + 1].SANmv);
        }
    }
    else {

      printf ("  1.          %-7s\n", Game[0].SANmv);

      for (i = 1; i <= GameCnt; i += 2)
        {
          printf ("%3d.  %-7s %-7s\n", i/2 + 2, Game[i].SANmv,
    	      Game[i + 1].SANmv);
        }
    }
    printf ("\n");
  }
}
