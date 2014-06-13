# The source directory.
SRCDIR=src
# The source directory.
LIBDIR=lib
# The include directory
INCDIR=inc
# The extension you are using for C++ source files. Options include: cpp, cc, cxx.
CPP_EXT=cpp
OPTS = -I"/usr/include" -L"/usr/lib"
# What you run to play the game!
EXECUTABLE=main.out
# The directory to put the executable.
BINDIR=bin
# The C++ compiler you are using.
CC=g++
# The packages loaded by pkg-config.
PKGS=pkg-config --cflags --static --libs glfw3 assimp freetype2 libpng
LIBS=-lxcursor -lm -lGLESv2 -lftgl
# The directory to store the compiled .o files.
OBJDIR=obj
# Optimization Level - O1 is "fast" optimize. O2 include all opts that do not take forever + do not have space tradeoffs
# O3 is full optimization but creates a large file and takes awhile. Os is the same as O2 but runs a second pass that reduces filesize afterwards
OPTLEVEL =-O2

# Mac frameworks
FRAMEWORKS=

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
OPTS += ./lib/linux-gcc-64/libIrrKlang.so
else
OPTS += ./lib/macosx-gcc/libirrklang.dylib
FRAMEWORKS += -DGL_GLEXT_PROTOTYPES -framework OpenGL -framework ApplicationServices
endif

# The flags you want to use when compiling individual objects (.o files)
# -g=generate information for debugging (used with debugging tools such as gdb
#  or valgrind)
# -Wall=Give me all warnings!
# -Wextra=Give me extra warnings!
# -std=c++11=Enforce C++11 standard compliance.
# -MMD=Create a .d file to store the rule for the header dependencies of each object.
CFLAGS=-g $(OPTLEVEL) -Wall -Wextra -Wno-write-strings -std=c++0x -MMD -I$(SRCDIR) -I$(LIBDIR) -I$(INCDIR) `pkg-config --cflags $(PKGS)` $(LIBS)

# LDLIBS (Load Libraries)
# External libraries you are using that need to be linked.
# ``=run a shell command (command substitution)
# sdl-config=a command that generates the load libs/cflags necessary depending
# on the platform (OS/Linux/Win)
LDLIBS=`pkg-config --libs $(PKGS)` -lftgl

# LDFLAGS (Load/linker flags)
LDFLAGS=

# Pretty much everything past this point is not really meant to be modified.
# But have at it if you want.

# The C++ source files (but not the headers)
SRCS=$(shell find $(SRCDIR) -type f -name "*.$(CPP_EXT)")

# The .o files. One per cpp file. Put them in the obj directory.
OBJECTS=$(patsubst %.$(CPP_EXT),$(OBJDIR)/%.o,$(SRCS))

# If files named all/clean/run exist in the directory, then
# Make will not run these commands (because they will be "Up-to-Date")
# .PHONY is a special way of telling Make to run these no matter what.
.PHONY: all clean run

all: $(EXECUTABLE) $(REDUCER_EXEC)

# Pastes in all of the Make rules inside of the generated .d files.
# Rules are of the format (file.o: file.cpp header1.h header2.h)
# Each .d file gets generated whenever its .o file is compiled.
# Special thanks to /u/yurik for pointing this out to me!
-include $(OBJECTS:.o=.d)

# Links the objects into a "cavestory" executable.
# Also links the libs in $(LDLIBS) (SDL1.2 in our case)
# Puts the Executable in the bindir.
# re-link when an object or dependency has changed.
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $(FRAMEWORKS) -o $(BINDIR)/$(EXECUTABLE) $(OBJECTS) $(LDLIBS) $(OPTS) 

# The rule for how to compile an .o (object) file from a .cpp (C++ source) file
# "-c" creates a .o file
# "-o" says to create an object file (.o)
# "$@" in this case is the name of the object file (.o). MUST FOLLOW -o
# "$<" in this case is the .cpp file
# Make the directory if it doesn't exist
$(OBJDIR)/%.o: %.$(CPP_EXT)
	@mkdir -p $(@D)
	$(CC) $(FRAMEWORKS) -c $(CFLAGS) -o $@ $<

# Deletes all .o/.d files and the executable. This helps when you want to
# force recompilation.
clean:
	rm -rf $(OBJDIR) $(BINDIR)

cleanMods:
	rm -f resources/*.mod

# Just a nice way of running the game. (Since you have to be in the bindir for
# the content to load properly)
run:
	$(BINDIR)/$(EXECUTABLE)

# Run this to remake the mod files
mod:
	$(BINDIR)/$(EXECUTABLE) -remake
