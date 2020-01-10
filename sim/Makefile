CC              := g++
CXXFLAGS        := -std=c++17 -Wall -Wextra -pedantic -O3

SRCDIR          := src
BUILDDIR        := build
BINDIR          := bin
SOURCES         := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS         := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

DEPDIRS         := ../types ../game
DEPINCLDIRS     := $(DEPDIRS:%=%/include)
DEPSRCS         := $(foreach dir,$(DEPDIRS),$(wildcard $(dir)/src/*.cpp))
DEPOBJS         := $(foreach src,$(DEPSRCS), \
						$(dir $(src))../build/$(basename $(notdir $(src))).o)
				# also track the header files
DEPSRCS         += $(foreach dir,$(DEPDIRS),$(wildcard $(dir)/include/*.hpp))

TARGET          := $(BINDIR)/sim
INCLUDE         := -I$(SRCDIR) $(DEPINCLDIRS:%=-I%)
EXAMPLEARGS     := --cycles 50000 --every 5000 --stats --report map
EXAMPLESLOWARGS := --cycles 250 --every 1 --slow --stats --report map
EXAMPLEDBGARGS  := --cycles 100 --every 1 --stats --report debug
EXAMPLEFILES    := data/example/example.world \
			       data/example/example1.bug \
			       data/example/example2.bug

LOGEXT          := log

all: $(TARGET)

example: $(TARGET)
	$< $(EXAMPLEARGS) $(EXAMPLEFILES)

example_slow: $(TARGET)
	$< $(EXAMPLESLOWARGS) $(EXAMPLEFILES)

example_debug: $(TARGET)
	$< $(EXAMPLEDBGARGS) $(EXAMPLEFILES)

$(TARGET): $(OBJECTS) $(DEPSRCS)
	@mkdir -p $(BINDIR)
	for dep in $(DEPDIRS);   \
	do                       \
		$(MAKE) -s -C $$dep; \
	done
	$(CC) $(CXXFLAGS) $(INCLUDE) $(OBJECTS) $(DEPOBJS) -o $@

$(OBJECTS): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f *.$(LOGEXT)
	rm -rf $(BUILDDIR) $(BINDIR)

cleanall: clean
	for dep in $(DEPDIRS);         \
	do                             \
		$(MAKE) -s -C $$dep clean; \
	done

.PHONY: all example example_slow example_debug \
		$(DEPDIRS) $(DEPCLEAN) clean cleanall