# Process this file with automake to produce Makefile.in (in this,
# and all subdirectories).
# Makefile for the top-level directory of GNU Chess.
#
# Copyright (C) 2001-2013 Free Software Foundation, Inc.
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

# Find gnulib headers.
ACLOCAL_AMFLAGS = --install -I m4

# Subdirectories to descend into.
SUBDIRS = src doc po

dist_pkgdata_DATA = src/gnuchess.ini src/smallbook.bin

# Additional files to distribute.
EXTRA_DIST = config.rpath m4/ChangeLog  TODO src/gnuchess.ini src/smallbook.bin src/gnuchessx src/gnuchessu doc/quotes.rb lib/gettext.h

DISTCLEANFILES = *~ src/GCint.h
pkgdatadir =$(datadir)/gnuchess

AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_builddir)/lib
