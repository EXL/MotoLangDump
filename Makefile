# This Makefile was edited by EXL, 23-Jul-2022
# Default MotoMAGX platform is Motorola ZINE ZN5

####### Config
APPNAME     := MotoLangDump_magx

####### SDK and Toolchains
TOOLPREFIX  := /arm-eabi
ARMLIB      := $(TOOLPREFIX)/arm-linux-gnueabi/lib

####### Default Platform
QTDIR       := $(TOOLPREFIX)/lib/qt-zn5
EZXDIR      := $(TOOLPREFIX)/lib/ezx-zn5
LINKLIB     := -lezxappbase -lqte-mt
TARGET       = $(APPNAME)

####### Build Tools and Options
MOC          = $(QTDIR)/bin/moc
CC           = $(TOOLPREFIX)/bin/arm-linux-gnueabi-gcc
CXX          = $(TOOLPREFIX)/bin/arm-linux-gnueabi-g++
STRIP        = $(TOOLPREFIX)/bin/arm-linux-gnueabi-strip
INCPATH      = -I. -I$(QTDIR)/include -I$(EZXDIR)/include -I$(TOOLPREFIX)/arm-linux-gnueabi/include
CFLAGS       = -pipe -Wall -W -O2 -DNO_DEBUG $(INCPATH)
CXXFLAGS     = -pipe -DQWS -fno-exceptions -fno-rtti -Wall -W -O2 -DNO_DEBUG $(INCPATH)
LDLAGS       = -Wl,-rpath-link,$(EZXDIR)/lib
LIBS         = -L$(EZXDIR)/lib -L$(ARMLIB) -L$(QTDIR)/lib $(LINKLIB)

####### Files
SOURCES      = MotoLangDump.cpp
OBJECTS      = MotoLangDump.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	$(STRIP) -s $(TARGET)

tar: $(TARGET)
	-tar -cvf $(TARGET).tar $(TARGET)

rmobj:
	-rm -f $(OBJECTS)
	-rm -f *~ core
	$(RM) *.o

clean: rmobj
	$(RM) $(TARGET) *.tar
