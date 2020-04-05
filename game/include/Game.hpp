#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <string>

#define DEBUG_HEADER_EVERY 18 // 0 disables
#define SLOW_MODE_SLEEP_MILLISECONDS 125

class World;

class Game
{
    public:
    std::unique_ptr<World> this_world_ptr; // needs ~World
    Game(std::string, std::string, std::string, bool keep_log=true);
    ~Game(); // defined =default in the implementation
    void simulate(int, bool, bool, int, std::string, bool);
    std::pair<int, int> get_score(void);
    static void print_stats(World &);
    static void print_state(std::string, World &, int);
    static void print_map_state(World &);
    static void print_debug_state(World &);
    static void print_debug_header(bool repeat=false);
};

#endif
