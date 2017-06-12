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

libfrontend_a_SOURCES = atak.cc cmd.cc debug.cc epd.cc genmove.cc \
 init.cc move.cc output.cc players.cc\
 pgn.cc solve.cc \
 swap.cc util.cc engine.cc common.h \
 inlines.h version.h lexpgn.ll lexpgn.h input.cc

AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/lib

AM_LDFLAGS = $(PTHREAD_LDFLAGS) $(PTHREAD_LIBS)

DISTCLEANFILES = *~

