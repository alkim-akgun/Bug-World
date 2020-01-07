#include "types.hpp"
#include "Bug.hpp"

tcolor other_color(tcolor);
bool are_friends(Bug &, Bug &);
tposition adjacent(tposition, tdirection);
tdirection turn(tleftright, tdirection);
tposition sensed_position(tposition, tdirection, tsensedir);
void unexpected_error(std::string arg = "");