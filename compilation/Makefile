
TARGET = PSP_TARGET
OBJS = ../src/callbacks.o ../main.o

LIBS = ../compilation/pspp.a

INCDIR = ../include/pspp/ ../include/
CFLAGS = -G0 -O2 -Wall
CXXFLAGS = $(CFLAGS) -fno-rtti -fexceptions
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Prototype

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
