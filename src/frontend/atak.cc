/* atak.c

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
#include <string.h>
#include "common.h"


short SqAtakd (short sq, short side)
/**************************************************************************
 *
 *  To determine if sq is attacked by any pieces from side.
 *
 **************************************************************************/
{
   register BitBoard *a, b, *c, d, blocker;
   int t;
   
   a = board.b[side];

   /* Knights */
   if (a[knight] & MoveArray[knight][sq])
      return (true);

   /* Kings */
   if (a[king] & MoveArray[king][sq])
      return (true);

   /* Pawns */
   if (a[pawn] & MoveArray[ptype[1^side]][sq])
      return (true);
      
   c = FromToRay[sq];
   blocker = board.blocker;

   /* Bishops & Queen */
   b = (a[bishop] | a[queen]) & MoveArray[bishop][sq];
   d = ~b & blocker; 
   while (b)
   {
      t = leadz (b);
      if (!(c[t] & d))
         return (true);
      CLEARBIT (b, t); 
   }

   /* Rooks & Queen */
   b = (a[rook] | a[queen]) & MoveArray[rook][sq];
   d = ~b & blocker;
   while (b)
   {
      t = leadz (b);
      if (!(c[t] & d))
         return (true);
      CLEARBIT (b, t); 
   }
   return (false);
}

BitBoard AttackTo (int sq, int side)
/***************************************************************************
 *
 *  Generate a bitboard of all squares with pieces belonging to side
 *  which attack sq.
 *
 ***************************************************************************/
{
   register BitBoard *a, b, *c, e, blocker;
   int t;
   
   a = board.b[side];

   /* Knights */
   e = (a[knight] & MoveArray[knight][sq]); 	

   /* Kings */
   e |= (a[king] & MoveArray[king][sq]); 	

   /* Pawns */
   e |= (a[pawn] & MoveArray[ptype[1^side]][sq]);
      
   c = FromToRay[sq];
   blocker = board.blocker;

   /* Bishops & Queen */
   b = (a[bishop] | a[queen]) & MoveArray[bishop][sq];
   while (b)
   {
      t = leadz (b);
      CLEARBIT (b, t); 
      if (!(c[t] & blocker & NotBitPosArray[t]))
	 e |= BitPosArray[t];
   }

   /* Rooks & Queen */
   b = (a[rook] | a[queen]) & MoveArray[rook][sq];
   while (b)
   {
      t = leadz (b);
      CLEARBIT (b, t); 
      if (!(c[t] & blocker & NotBitPosArray[t]))
	 e |= BitPosArray[t];
   }

   return (e);
}
 

int PinnedOnKing (int sq, int side)
/***************************************************************************
 *
 *  Determine if the piece on sq is pinned against the King.
 *  Side is the color of the piece.  
 *  Caveat: PinnedOnKing should only be called by GenCheckEscapes().
 *  The more generic FindPins() function should be used for evaluating
 *  pins against other pieces.
 *
 ***************************************************************************/
{
   int xside;
   int KingSq, dir, sq1;
   BitBoard b, blocker;

   KingSq = board.king[side];
   if ((dir = directions[KingSq][sq]) == -1)
      return (false);

   xside = 1 ^ side;
   blocker = board.blocker;
 
   /*  Path from piece to king is blocked, so no pin */
   if (FromToRay[KingSq][sq] & NotBitPosArray[sq] & blocker)
      return (false);
   b = (Ray[KingSq][dir] ^ FromToRay[KingSq][sq]) & blocker;
   if (b == NULLBITBOARD)
      return (false);
   sq1 = (sq > KingSq ? leadz (b) : trailz (b));

   /*  If diagonal  */
   if (dir <= 3 && 
	BitPosArray[sq1] & (board.b[xside][queen] | board.b[xside][bishop]))
      return (true);
   
   /*  Rank / file  */  
   if (dir >= 4 && 
	BitPosArray[sq1] & (board.b[xside][queen] | board.b[xside][rook]))
      return (true);

   return (false);
}

