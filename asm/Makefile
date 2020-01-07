CC              := g++
CXXFLAGS        := -std=c++17 -Wall -Wextra -pedantic -O3

SRCDIR          := src
BUILDDIR        := build
BINDIR          := bin
SOURCES         := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS         := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

DEPDIRS         := ../types
DEPINCLDIRS     := $(DEPDIRS:%=%/include)
DEPSRCS         := $(foreach dir,$(DEPDIRS),$(wildcard $(dir)/src/*.cpp))
DEPOBJS         := $(foreach src,$(DEPSRCS), \
						$(dir $(src))../build/$(basename $(notdir $(src))).o)
				# also track the header files
DEPSRCS         += $(foreach dir,$(DEPDIRS),$(wildcard $(dir)/include/*.hpp))

TARGET          := $(BINDIR)/asm
INCLUDE         := -I$(SRCDIR) $(DEPINCLDIRS:%=-I%)
EXAMPLE         := data/example/example.buggy

LOGEXT          := log

all: $(TARGET)

example: $(EXAMPLE) $(TARGET)
	$(TARGET) $<

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
	rm -rf $(BUILDDIR) $(BINDIR)

cleanall: clean
	for dep in $(DEPDIRS);         \
	do                             \
		$(MAKE) -s -C $$dep clean; \
	done

.PHONY: all example clean cleanall