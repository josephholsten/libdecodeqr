/////////////////////////////////////////////////////////////////////////
//
// qrerror.h --a part of libdecodeqr
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
#ifndef __QR_ERROR_CODE__
#define __QR_ERROR_CODE__

#define QR_IMAGEREADER_WORKING 0x1000
#define QR_IMAGEREADER_DECODED 0x2000

#define QR_VERSIONINFO_ERROR 0x0f00
#define QR_VERSIONINFO_INVALID 0x0100
#define QR_VERSIONINFO_MISMATCH 0x0200
#define QR_VERSIONINFO_UNRECOVERABLE 0x0800

#define QR_FORMATINFO_ERROR 0x00f0
#define QR_FORMATINFO_INVALID_LEVEL 0x0010
#define QR_FORMATINFO_UNRECOVERABLE 0x0080

#define QR_CODEDATA_ERROR 0x000f
#define QR_CODEDATA_NOT_SUPPORT_ECI 0x0001
#define QR_CODEDATA_LENGTH_MISMATCH 0x0002
#define QR_CODEDATA_UNRECOVERABLE 0x0008

#define QR_IMAGEREADER_ERROR 0x4000
#define QR_IMAGEREADER_NOT_INVALID_SRC_IMAGE 0x0100
#define QR_IMAGEREADER_NOT_FOUND_FINDER_PATTERN 0x0200
#define QR_IMAGEREADER_NOT_FOUND_CODE_AREA 0x0400
#define QR_IMAGEREADER_NOT_DETERMINABLE_CODE_AREA 0x0800

#endif
