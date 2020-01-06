CC         := g++
CFLAGS     := -std=c++17 -Wall -Wextra -pedantic

SRCDIR     := src
BUILDDIR   := build
BINDIR     := bin
SOURCES    := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS    := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

DEPDIR     := ../types
DEPINCLDIR := $(DEPDIR)/include
DEPSRCDIR  := $(DEPDIR)/src
DEPSRC     := $(wildcard $(DEPSRCDIR)/*.cpp)
DEPOBJS    := $(DEPSRC:$(DEPSRCDIR)/%.cpp=$(BUILDDIR)/%.o)

TARGET     := $(BINDIR)/asm
INCLUDE    := -I$(SRCDIR) -I$(DEPINCLDIR)
EXAMPLE    := data/example/example.buggy

all: $(TARGET)

example: $(EXAMPLE) $(TARGET)
	$(TARGET) $<

$(TARGET): $(OBJECTS) $(DEPOBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJECTS) $(DEPOBJS) -o $@

$(OBJECTS): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(DEPOBJS): $(BUILDDIR)/%.o: $(DEPSRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all example clean
