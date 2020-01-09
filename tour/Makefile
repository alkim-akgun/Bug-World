CC              := g++
CXXFLAGS        := -std=c++17 -Wall -Wextra -pedantic -O3 -pthread

SRCDIR          := src
BUILDDIR        := build
BINDIR          := bin
SOURCES         := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS         := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

DEPDIRS         := ../game ../types
DEPINCLDIRS     := $(DEPDIRS:%=%/include)
DEPSRCS         := $(foreach dir,$(DEPDIRS),$(wildcard $(dir)/src/*.cpp))
DEPOBJS         := $(foreach src,$(DEPSRCS), \
						$(dir $(src))../build/$(basename $(notdir $(src))).o)
				# also track the header files
DEPSRCS         += $(foreach dir,$(DEPDIRS),$(wildcard $(dir)/include/*.hpp))

TARGET          := $(BINDIR)/tour
INCLUDE         := -I$(SRCDIR) $(DEPINCLDIRS:%=-I%)
LIBRARY         := -lstdc++fs

TOURDIR         := ./data
TOURARGS        := --cycles 100000

EXAMPLETOURDIR  := ./data/example
EXAMPLEARGS     := --cycles 50000

all: $(TARGET)

example: $(TARGET)
	$< $(EXAMPLEARGS) $(EXAMPLETOURDIR)

tournament: $(TARGET)
	$< $(TOURARGS) $(TOURDIR)

$(TARGET): $(OBJECTS) $(DEPSRCS)
	@mkdir -p $(BINDIR)
	for dep in $(DEPDIRS);   \
	do                       \
		$(MAKE) -s -C $$dep; \
	done
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) $(DEPOBJS) $(LIBRARY) -o $@

$(OBJECTS): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

cleanall: clean
	for dep in $(DEPDIRS);         \
	do                             \
		$(MAKE) -s -C $$dep clean; \
	done

.PHONY: all example tournament clean cleanall
