#ifndef TYPES_HPP
#define TYPES_HPP

#include <utility>
#include <string>
#include <iosfwd>
#include <stdexcept>

// sim only
enum class tcolor {RED = 0, BLACK = 1, NONE = -1};

typedef int tstate; // non-negative

// position reflects the indices that point to the cell in world
// first coordinate concerns the vertical axis
// second coordinate concerns the horizontal axis
typedef std::pair<int, int> tposition;

// asm and sim
enum class tcondition { MARKER0, // first 6 tconditions correspond to marker values
                        MARKER1,
                        MARKER2,
                        MARKER3,
                        MARKER4,
                        MARKER5,
                        FRIEND,
                        FOE,
                        FRIENDWITHFOOD,
                        FOOWITHFOOD,
                        FOOD,
                        ROCK,
                        HOME,
                        FOEHOME,
                        FOEMARKER,
                        Count   };

enum class tdirection { EAST,
                        SOUTHEAST,
                        SOUTHWEST,
                        WEST,
                        NORTHWEST,
                        NORTHEAST,
                        Count     };

enum class tleftright { LEFT, RIGHT, Count };

enum class tsensedir { HERE, AHEAD, LEFTAHEAD, RIGHTAHEAD, Count };

typedef int tmark; // {0, 1, 2, 3, 4, 5} must be checked

namespace std
{
    string to_string(tposition);
    string to_string(tcondition);
    string to_string(tdirection);
    string to_string(tleftright);
    string to_string(tsensedir);
    string to_string(tcolor);
}

std::istream & operator>>(std::istream &, tcondition &);
std::istream & operator>>(std::istream &, tdirection &);
std::istream & operator>>(std::istream &, tleftright &);
std::istream & operator>>(std::istream &, tsensedir &);

#endif