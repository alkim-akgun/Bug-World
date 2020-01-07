CC              := g++
CXXFLAGS        := -std=c++17 -Wall -Wextra -pedantic -O3

SRCDIR          := src
INCLDIR         := include
BUILDDIR        := build
SOURCES         := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS         := $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

INCLUDE         := -Iinclude

all: $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean