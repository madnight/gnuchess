/* inlines.h

   GNU Chess frontend

   This file constains static inline version of performance-critical
   functions. Is included by common.h unless NO_INLINE is defined.

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

#ifndef INLINES_H
#define INLINES_H

static inline unsigned char leadz (BitBoard b)
/**************************************************************************
 *
 *  Returns the leading bit in a bitboard.  Leftmost bit is 0 and
 *  rightmost bit is 63.  Thanks to Robert Hyatt for this algorithm.
 *
 ***************************************************************************/
{
  if (b >> 48) return lzArray[b >> 48];
  if (b >> 32) return lzArray[b >> 32] + 16;
  if (b >> 16) return lzArray[b >> 16] + 32;
  return lzArray[b] + 48;
}


static inline unsigned char nbits (BitBoard b)
/***************************************************************************
 *
 *  Count the number of bits in b.
 *
 ***************************************************************************/
{
  return BitCount[b>>48] + BitCount[(b>>32) & 0xffff]
    + BitCount[(b>>16) & 0xffff] + BitCount[b & 0xffff];
}

#endif /* !INLINES_H */
