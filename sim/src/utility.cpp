#include "utility.hpp"

#include <string>

tcolor other_color(tcolor col)
{
    // 0 -> 1; 1 -> 0
    return (tcolor)(1 - (int)col); // only works with 2 colors
}

bool are_friends(Bug & b1, Bug & b2)
{
    return (b1.get_color() == b2.get_color());
}

tposition adjacent(tposition pos, tdirection dir)
{
    int x = pos.first; // vertical
    int y = pos.second; // horizontal
    bool even = (x % 2 == 0);
    switch (dir)
    {
        case tdirection::EAST:
            return tposition(x, y+1);
            break;
        case tdirection::SOUTHEAST:
            return (even ? tposition(x+1, y) : tposition(x+1, y+1));
            break;
        case tdirection::SOUTHWEST:
            return (even ? tposition(x+1, y-1) : tposition(x+1, y));
            break;
        case tdirection::WEST:
            return tposition(x, y-1);
            break;
        case tdirection::NORTHWEST:
            return (even ? tposition(x-1, y-1) : tposition(x-1, y));
            break;
        case tdirection::NORTHEAST:
            return (even ? tposition(x-1, y) : tposition(x-1, y+1));
            break;
        default:
            std::string msg = "invalid tdirection "
                            + std::to_string((int)dir);
            throw std::runtime_error(msg);
            break;       
    }
}

tdirection turn(tleftright lr, tdirection dir)
{
    if (lr == tleftright::LEFT)
        return static_cast<tdirection>(((int)dir + 5) % 6);
    else if (lr == tleftright::RIGHT)
        return static_cast<tdirection>(((int)dir + 1) % 6);
    else
        unexpected_error("utility.cpp: turn");
    return tdirection::Count;
}

tposition sensed_position(tposition pos, tdirection dir, tsensedir sd)
{
    switch (sd)
    {
        case tsensedir::HERE:
            return pos;
            break;
        case tsensedir::AHEAD:
            return adjacent(pos, dir);
            break;
        case tsensedir::LEFTAHEAD:
            return adjacent(pos, turn(tleftright::LEFT, dir));
            break;
        case tsensedir::RIGHTAHEAD:
            return adjacent(pos, turn(tleftright::RIGHT, dir));
            break;
        default:
            std::string msg = "invalid sensedir " + std::to_string(sd);
            throw std::runtime_error(msg);
    }
}

void unexpected_error(std::string arg)
{
    std::string msg = "unexpected error: " + arg;
    throw std::runtime_error(msg);
}