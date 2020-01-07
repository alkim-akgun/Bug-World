#ifndef BUG_HPP
#define BUG_HPP

#include "types.hpp"

class Bug
{
    private:
    tposition pos = tposition(-1, -1);
    tstate state = 0;
    tdirection direction = tdirection::EAST;
    int remaining_rest = 0;
    bool food = 0;
    bool dead = false;
    // initialized in constructor initializer list - keep the order
    const int id;
    const tcolor color;
    const int prog_id, resting;
    

    public:
    Bug(int, tcolor, int, int);
    inline void start_resting(void) { this->remaining_rest = this->resting; }
    inline bool rested(void) const { return this->remaining_rest == 0; }
    inline int get_remaining_rest(void) { return this->remaining_rest; }
    inline void rest(void) { this->remaining_rest--; }
    inline int get_id(void) const { return this->id; }
    inline int get_progid(void) const { return this->prog_id; }
    inline tcolor get_color(void) const { return this->color; }
    inline tstate get_state(void) const {return this->state; }
    inline void set_state(tstate s) { this->state = s; }
    inline tdirection get_direction(void) const { return this->direction; }
    inline void set_direction(tdirection dir) { this->direction = dir; }
    inline tposition get_position(void) const { return this->pos; }
    inline void set_position(tposition p) { this->pos = p; }
    inline bool get_food(void) const {return this->food; }
    inline void set_food(bool f) { this->food = f; }
    inline bool is_dead(void) const { return this->dead; }
    inline void kill(void) { this->dead = true; }
};

#endif