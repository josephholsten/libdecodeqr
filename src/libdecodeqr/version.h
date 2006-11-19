/////////////////////////////////////////////////////////////////////////
//
// version.h --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id: version.h 6 2006-11-18 09:37:16Z zophos $
//
#ifndef __QR_LIBDECODEQR_VERSION__
#define __QR_LIBDECODEQR_VERSION__

#define LIBDECODEQR_VERSION_MAJOR 0
#define LIBDECODEQR_VERSION_MINOR 9
#define LIBDECODEQR_VERSION_TEENY 2
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
#endif
