
TARGET = PSP_TARGET
OBJS = ../src/callbacks.o ../src/ui.o ../main.o

LIBS = ../compilation/cyoa.a ../compilation/pspp.a

INCDIR = ../include/cyoa/ ../include/pspp/ ../include/
CFLAGS = -G0 -O2 -Wall
CXXFLAGS = $(CFLAGS) -fno-rtti -fexceptions
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Prototype

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
