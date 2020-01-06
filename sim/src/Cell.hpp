#ifndef CELL_HPP
#define CELL_HPP

#include "Bug.hpp"
#include "Marker.hpp"

class Cell
{
    private:
    Bug* occupant = nullptr;
    bool obstructed = false;
    int food = 0;
    bool home_red_bug = false;
    bool home_black_bug = false;

    public:
    Cell(void) = default;
    Cell(char);
    Marker marks;
    inline bool is_occupied(void) const { return this->occupant != nullptr; }
    inline Bug* get_occupant_ptr(void) const { return this->occupant; }
    inline void set_occupant_ptr(Bug* b) { this->occupant = b; }
    inline int get_food(void) const { return this->food; }
    inline void set_food(int f) { this->food = f; }
    inline bool get_obstructed(void) const { return this->obstructed; }
    inline bool is_black_home_area(void) const { return this->home_black_bug; }
    inline bool is_red_home_area(void) const { return this->home_red_bug; }
    bool is_home_to_color(tcolor) const;
};

#endif
