#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(libdecodeqr, 0.9.4, zophos@koka-in.org)
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([libdecodeqr/bitstream.cpp])

# Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL
AC_CHECK_TOOL(AR,ar)
AC_CHECK_TOOL(LD,ld)

#
# Check if don't want -g option
#
AC_ARG_ENABLE(optimize,
       [  --enable-optimize       turn off debug information (CC -g option). ],
       [enable_optimize=$enableval], [enable_optimize=no])
if test x"$enable_optimize" = xyes; then
   CXXFLAGS="$CXXFLAGS -O2"
fi

AC_ARG_ENABLE(debug,
       [  --enable-debugmessage   turn on debug message.],
       [enable_debugmessage=$enableval], [enable_debugmessage=no])
if test x"$enable_debugmessage" = xyes; then
   CPPFLAGS="$CPPFLAGS -D_DEBUG"
fi

PKG_CHECK_MODULES([DEPS], [opencv])
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LDFLAGS)

# Checks for header files.
AC_CHECK_HEADERS([memory.h netinet/in.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([memset memcpy snprintf])
# Get version info
# DECODEQR_VERSION(ID)
# ----------------------------
# ID should be MAJOR, MINOR, or TEENY
AC_DEFUN([DECODEQR_VERSION], [
  $1=`grep '#define LIBDECODEQR_VERSION_$1' $srcdir/libdecodeqr/version.h | sed 's/#define LIBDECODEQR_VERSION_$1 \([0-9]*\)/\1/'`
  AC_SUBST($1)
])

DECODEQR_VERSION(MAJOR)
DECODEQR_VERSION(MINOR)
DECODEQR_VERSION(TEENY)

AC_CONFIG_FILES([
	Makefile
    libdecodeqr/Makefile
    examples/Makefile
    examples/simple/Makefile
    examples/webcam/Makefile
    test/Makefile
])
AC_OUTPUT
