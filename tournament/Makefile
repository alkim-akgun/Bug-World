CC              := g++
CXXFLAGS        := -std=c++17 -Wall -Wextra -pedantic -O3 -pthread

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

TARGET          := $(BINDIR)/tour
INCLUDE         := -I$(SRCDIR) $(DEPINCLDIRS:%=-I%)
EXAMPLEARGS     := --cycles 50000
EXAMPLEFILES    := data/example/example.world \
			       data/example/example.bug \
			       data/example/example.bug \
						 data/example/example.bug

LOGEXT          := log

all: $(TARGET)

example: $(TARGET)
	$< $(EXAMPLEARGS) $(EXAMPLEFILES)

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
