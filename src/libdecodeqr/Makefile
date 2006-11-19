#!/usr/bin/make
#
# Makefile -- a part of libdecodeqr
#
# Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
#
# This is free software with ABSOLUTELY NO WARRANTY.
# You can redistribute and/or modify it under the terms of GPLv2.
#
# $Id$
#
MAJOR=0
MINOR=9
TEENY=2

BASENAME=libdecodeqr
LIBNAME_A=$(BASENAME).a
SONAME=$(BASENAME).so.$(MAJOR)
LIBNAME_SO=$(BASENAME).so.$(MAJOR).$(MINOR).$(TEENY)


OPENCV_INCLUDE=/usr/local/include/opencv
INCLUDE=-I $(OPENCV_INCLUDE)

CC=gcc
CC_OPT=-fPIC -c -x 'c++' $(INCLUDE)
LD=gcc
LD_OPT=-shared -Wl,-soname,$(SONAME) -o $(LIBNAME_SO)
AR=ar
AR_OPT=rcs

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CC_OPT) $<

headers:=$(wildcard *.h)
sources:=$(wildcard *.cpp)
objs:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(LIBNAME_A) $(LIBNAME_SO)

$(LIBNAME_A): $(objs)
	$(AR) $(AR_OPT) $(LIBNAME_A) $(objs)

$(LIBNAME_SO): $(objs)
	$(LD) $(LD_OPT) $(objs)

clean:
	-rm *.a *.o *.so.*