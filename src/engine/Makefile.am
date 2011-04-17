# Process this file with automake to produce Makefile.in 
# Makefile for gnuchess/src/engine.
#
# Copyright (C) 2001-2011 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

noinst_LIBRARIES = libengine.a

libengine_a_SOURCES = attack.cpp board.cpp book.cpp eval.cpp fen.cpp hash.cpp list.cpp main.cpp material.cpp \
       move.cpp move_check.cpp move_do.cpp move_evasion.cpp move_gen.cpp move_legal.cpp \
       option.cpp pawn.cpp piece.cpp posix.cpp protocol.cpp pst.cpp pv.cpp random.cpp recog.cpp \
       search.cpp search_full.cpp see.cpp sort.cpp square.cpp trans.cpp util.cpp value.cpp \
       vector.cpp \
       attack.h board.h book.h colour.h eval.h fen.h hash.h list.h material.h \
       move.h move_check.h move_do.h move_evasion.h move_gen.h move_legal.h \
       option.h pawn.h piece.h posix.h protocol.h pst.h pv.h random.h recog.h \
       search.h search_full.h see.h sort.h square.h trans.h util.h value.h \
       vector.h

AM_CPPFLAGS = -I$(top_srcdir)/src

AM_CXXFLAGS = $(PTHREAD_CXXFLAGS)

# Flags used to compile Fruit 2.1 - not used by default
# AM_CXXFLAGS += -fno-exceptions -fno-rtti -O3 -fstrict-aliasing -fomit-frame-pointer

AM_LDFLAGS = $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

DISTCLEANFILES = *~


