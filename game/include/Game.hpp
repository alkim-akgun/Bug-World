#ifndef GAME_HPP
#define GAME_HPP

#include <string>

#include "World.hpp"

#define debug_HEADER_EVERY 18 // 0 disables
#define SLOW_MODE_SLEEP_MILLISECONDS 125

class Game
{
    public:
    World this_world;
    Game(std::string, std::string, std::string);
    void simulate(int, bool, bool, int, std::string, bool silent);
    static void print_stats(World &);
    static void print_state(std::string, World &, int);
    static void print_map_state(World &);
    static void print_debug_state(World &);
    static void print_debug_header(bool repeat=false);
};

#endif