#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <string>

#include "Cell.hpp"
#include "Program.hpp"
#include "Bug.hpp"
#include "types.hpp"

// how many cycles a bug needs to rest
// after every movement action
#define REST_NEEDED_PER_MOVE 14

class World
{
    private:
    int width, height;
    bool keep_log = true;
    bool loaded = false;
    std::vector<std::vector<Cell>> cells;
    Program programs[2];
    std::vector<Bug> bugs;
    int adjacent_other_bugs(tposition, tcolor);
    void kill_at(tposition);

    public:
    World(void) = default;
    inline World(bool keep_log=true) : keep_log(keep_log) { }
    inline int get_width(void) const { return this->width; }
    inline int get_height(void) const { return this->height; }
    void load(std::string, std::string, std::string);
    void execute_cycle(int);

    Cell & get_cell(tposition);
    bool is_valid_position(tposition) const;
    bool kill_if_surrounded(tposition);
    void check_for_surrounded_bugs(tposition);

    tcolor winner(void) const;
    int red_food(void) const;
    int black_food(void) const;

    void error(std::string);
    void log(std::string);
};

#endif