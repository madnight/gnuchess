# Process this file with automake to produce Makefile.in 
# Makefile for gnuchess/src/frontend.
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

noinst_LIBRARIES = libfrontend.a

libfrontend_a_SOURCES = atak.c cmd.c debug.c epd.c genmove.c \
 init.c move.c output.c players.c\
 pgn.c solve.c \
 swap.c util.c engine.c common.h \
 inlines.h version.h lexpgn.l lexpgn.h

AM_CPPFLAGS = -I$(top_srcdir)/src

AM_CFLAGS = $(PTHREAD_CFLAGS) -std=c99

AM_LDFLAGS = $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

#CFLAGS += -std=c99

DISTCLEANFILES = *~

