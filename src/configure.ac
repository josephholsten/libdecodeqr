#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(libdecodeqr, package, zophos@koka-in.org)
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

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldecodeqr':
#AC_CHECK_LIB([decodeqr], [main])
AC_ARG_ENABLE(sample,
	[AS_HELP_STRING([--enable-sample],
	  [build sample programs])],
	[enable_sample=$withval],
	[enable_sample=no])
if test x"$enable_sample" != xno; then
   SAMPLE=sample
else
   SAMPLE=""
fi
AC_SUBST(SAMPLE)

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
libdecodeqr=$srcdir/libdecodeqr
decodeqr_major_version=`grep '#define LIBDECODEQR_VERSION_MAJOR' $libdecodeqr/version.h`
decodeqr_minor_version=`grep '#define LIBDECODEQR_VERSION_MINOR' $libdecodeqr/version.h`
decodeqr_teeny_version=`grep '#define LIBDECODEQR_VERSION_TEENY' $libdecodeqr/version.h`

MAJOR=`expr "$decodeqr_major_version" : '#define LIBDECODEQR_VERSION_MAJOR \([0-9]*\)'`
MINOR=`expr "$decodeqr_minor_version" : '#define LIBDECODEQR_VERSION_MINOR \([0-9]*\)'`
TEENY=`expr "$decodeqr_teeny_version" : '#define LIBDECODEQR_VERSION_TEENY \([0-9]*\)'`

AC_SUBST(MAJOR)
AC_SUBST(MINOR)
AC_SUBST(TEENY)

AC_CONFIG_FILES([
	Makefile
    libdecodeqr/Makefile
    sample/Makefile
    sample/simple/Makefile
    sample/webcam/Makefile
    test/Makefile
])
AC_OUTPUT
