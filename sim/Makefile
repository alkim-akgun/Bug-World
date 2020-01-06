CC              := g++
CFLAGS          := -std=c++17 -Wall -Wextra -pedantic -g

SRCDIR          := src
BUILDDIR        := build
BINDIR          := bin
SOURCES         := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS         := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

DEPDIR          := ../types
DEPINCLDIR      := $(DEPDIR)/include
DEPSRCDIR       := $(DEPDIR)/src
DEPSRC          := $(wildcard $(DEPSRCDIR)/*.cpp)
DEPOBJS         := $(DEPSRC:$(DEPSRCDIR)/%.cpp=$(BUILDDIR)/%.o)

TARGET          := $(BINDIR)/sim
INCLUDE         := -I$(SRCDIR) -I$(DEPINCLDIR)
EXAMPLEARGS     := --cycles 50000 --every 5000 --stats --report map
EXAMPLESLOWARGS := --cycles 250 --every 1 --slow --stats --report map
EXAMPLEDBGARGS  := --cycles 100 --every 1 --stats --report debug
EXAMPLEFILES    := data/example/example.world \
			       data/example/example.bug \
			       data/example/example.bug

LOGEXT          := log

all: $(TARGET)

example: $(TARGET)
	$< $(EXAMPLEARGS) $(EXAMPLEFILES)

example_slow: $(TARGET)
	$< $(EXAMPLESLOWARGS) $(EXAMPLEFILES)

example_debug: $(TARGET)
	$< $(EXAMPLEDBGARGS) $(EXAMPLEFILES)

$(TARGET): $(OBJECTS) $(DEPOBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJECTS) $(DEPOBJS) -o $@

$(OBJECTS): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR) $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(DEPOBJS): $(BUILDDIR)/%.o: $(DEPSRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR) $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f *.$(LOGEXT)
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all example example_slow example_debug clean
