# Process this file with automake to produce Makefile.in 
# Makefile for gnuchess/src/adapter.
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

noinst_LIBRARIES = libadapter.a

libadapter_a_SOURCES = adapter.cpp attack.cpp board.cpp book.cpp book_make.cpp book_merge.cpp colour.cpp \
       engine.cpp epd.cpp fen.cpp game.cpp hash.cpp io.cpp line.cpp list.cpp main.cpp move.cpp \
       move_do.cpp move_gen.cpp move_legal.cpp option.cpp parse.cpp pgn.cpp piece.cpp \
       posix.cpp random.cpp san.cpp search.cpp square.cpp uci.cpp util.cpp \
       adapter.h attack.h board.h book.h book_make.h book_merge.h colour.h \
       engine.h epd.h fen.h game.h hash.h io.h line.h list.h main.h move.h \
       move_do.h move_gen.h move_legal.h option.h parse.h pgn.h piece.h \
       posix.h random.h san.h search.h square.h uci.h util.h

AM_CPPFLAGS = -I$(top_srcdir)/src

AM_CXXFLAGS = $(PTHREAD_CXXFLAGS)

AM_LDFLAGS = $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

DISTCLEANFILES = *~

