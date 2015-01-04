dnl Process this file with autoconf to produce a configure script.
dnl
dnl Copyright (C) 2001-2015 Free Software Foundation, Inc.
dnl
dnl This file is free software; as a special exception the author gives
dnl unlimited permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
dnl implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([GNU Chess], [6.2.0], [bug-gnu-chess@gnu.org], [gnuchess])
AC_PREREQ(2.69)
AC_CONFIG_HEADERS(src/config.h)
AC_CONFIG_SRCDIR([src/main.cc])
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_LN_S
AC_STRUCT_TM
AM_PROG_LEX

dnl --with and --enable things
AC_ARG_WITH(readline, 
	AC_HELP_STRING([--with-readline],
	[use readline library if available (default is YES)]),
	ac_cv_use_readline=$withval,
	ac_cv_use_readline=yes)

dnl Check for readline library _and_ headers
if test x"$ac_cv_use_readline" = "xyes"; then
	AC_CHECK_LIB(ncurses, tputs)
	AC_CHECK_LIB(readline, readline)
	AC_CHECK_HEADERS(readline/readline.h readline/history.h)
fi

dnl Checks for header files.
AC_HEADER_STDC

dnl Either we should make these checks fail with an
dnl AC_MSG_ERROR or we should treat missing headers
dnl with #ifdef somehow. (As it stands, it still gives
dnl valuable debugging info for bug reports, but not more.)
AC_CHECK_HEADERS(time.h sys/time.h unistd.h errno.h fcntl.h libintl.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_VOLATILE
AC_FUNC_REALLOC
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AX_CREATE_STDINT_H(src/GCint.h)

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_FUNC_MALLOC
AC_FUNC_MEMCMP

dnl See above, how do we treat failure here?
AC_CHECK_FUNCS(gettimeofday strchr strcspn strstr strerror memset strdup strtol)

dnl Don't Check for getopt_long 
dnl just use bundled version like hello does
dnl AC_SUBST(LIBOBJS)
dnl AC_CHECK_FUNCS(getopt_long , , [LIBOBJS="$LIBOBJS getopt.o getopt1.o"] )
ACX_PTHREAD()

# GNU help2man creates man pages from --help output; in many cases, this
# is sufficient, and obviates the need to maintain man pages separately.
# However, this means invoking executables, which we generally cannot do
# when cross-compiling, so we test to avoid that (the variable
# "cross_compiling" is set by AC_PROG_CC).
if test $cross_compiling = no; then
  AM_MISSING_PROG(HELP2MAN, help2man)
else
  HELP2MAN=:
fi

# Support from GNU gettext.
AM_GNU_GETTEXT_VERSION([0.18.3])
AM_GNU_GETTEXT([external])

AC_ARG_WITH([pluginsdir],
            [AS_HELP_STRING([--with-pluginsdir=DIR],
                            [path where game data files get installed (default: $datadir/games/plugins)])],
            [PLUGINSDIR="$withval"],
            [PLUGINSDIR='$(datadir)/games/plugins'])

AC_SUBST(PLUGINSDIR)

AC_CONFIG_FILES(Makefile src/Makefile src/frontend/Makefile src/adapter/Makefile src/engine/Makefile doc/Makefile po/Makefile.in man/Makefile)
AC_OUTPUT
