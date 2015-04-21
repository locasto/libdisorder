#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libdisorder], [0.0.2])
AC_CONFIG_SRCDIR([src/disorder.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h math.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_LIB([m], [log2], , [CPPFLAGS=" -D__FreeBSD__ "])

# Optional features
AC_ARG_ENABLE([ropy],
	AS_HELP_STRING([--enable-ropy],
			[Enable ropy tool [default=yes]]))
AM_CONDITIONAL([ENABLE_ROPY], [test "x$enable_ropy" != "xno"])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tool/Makefile])
AC_OUTPUT
