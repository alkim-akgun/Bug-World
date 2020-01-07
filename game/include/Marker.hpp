#ifndef MARKER_HPP
#define MARKER_HPP

#include "types.hpp"

// basically a bitmask class
class Marker
{
    private:
    unsigned int bits = 0U;

    public:
    Marker(void) = default;
    inline unsigned int get_bits(void) const { return this->bits; } 
    static void validate_marker(tmark);
    void set_marker(tmark, tcolor);
    void clear_marker(tmark, tcolor);
    bool check_marker(tmark, tcolor) const;
    bool check_other_marker(tcolor) const;
};

#endif