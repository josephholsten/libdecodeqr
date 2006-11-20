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
OPENCV_INCLUDE=/usr/local/include/opencv
LIBDECODEQR_DIR=../../libdecodeqr

INCLUDE_DIR:=$(OPENCV_INCLUDE)
LIB_DIR:=$(LIBDECODEQR_DIR)

INCLUDE=-I$(INCLUDE_DIR)
LIBS=-ldecodeqr -lcv -lhighgui

CC=gcc
CC_OPT=-c -x 'c++'
LD=gcc
LD_OPT=

VPATH=./:$(LIBDECODEQR_DIR)

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CC_OPT) $(INCLUDE) $<

sources:=$(wildcard *.cpp)
objs:=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
dst:=webcam

all: $(dst)

$(dst): $(objs) libdecodeqr.a
	$(LD) $(LD_OPT) -o $@ $(objs) -L$(LIB_DIR) $(LIBS)

clean:
	-rm *.o $(dst)
