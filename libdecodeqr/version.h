/////////////////////////////////////////////////////////////////////////
//
// version.h --a part of libdecodeqr
//
// Copyright(C) 2007 NISHI Takao <zophos@koka-in.org>
//                   JMA  (Japan Medical Association)
//                   NaCl (Network Applied Communication Laboratory Ltd.)
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of LGPL.
//
// $Id$
//
#ifndef __QR_LIBDECODEQR_VERSION__
#define __QR_LIBDECODEQR_VERSION__

#define LIBDECODEQR_PRODUCTNAME "libdecodeqr"
#define LIBDECODEQR_VERSION_MAJOR 0
#define LIBDECODEQR_VERSION_MINOR 9
#define LIBDECODEQR_VERSION_TEENY 4
#define LIBDECODEQR_VERSION_SUFFIX "-dev"
#define LIBDECODEQR_VERSION_REVISION "$Rev$"

//
// a voodoo magic
//
#define LIBDECODEQR_VERSION_CAT(a,b,c,d,e) LIBDECODEQR_VERSION_CATX(a,b,c,d,e)
#define LIBDECODEQR_VERSION_CATX(a,b,c,d,e) #a "." #b "." #c d " (" e ")"

#define LIBDECODEQR_VERSION LIBDECODEQR_VERSION_CAT(\
    LIBDECODEQR_VERSION_MAJOR,\
    LIBDECODEQR_VERSION_MINOR,\
    LIBDECODEQR_VERSION_TEENY,\
    LIBDECODEQR_VERSION_SUFFIX,\
    LIBDECODEQR_VERSION_REVISION)

#define LIBDECODEQR_VERSION_DESCRIPTION \
    LIBDECODEQR_PRODUCTNAME " " LIBDECODEQR_VERSION

static const char *libdecodeqr_version_description=
    LIBDECODEQR_VERSION_DESCRIPTION;
#endif
