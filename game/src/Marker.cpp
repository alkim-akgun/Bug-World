#include "Marker.hpp"

#include "utility.hpp"

void Marker::validate_marker(tmark mark)
{
    if (mark < 0 || mark > 5)
    {
        std::string msg = "invalid tmark "
                        + std::to_string((int)mark);
        throw std::runtime_error(msg);
    }
}

// RED's marks start from 2^0
// BLACK's marks start from 2^6

void Marker::set_marker(tmark mark, tcolor col)
{
    Marker::validate_marker(mark);
    unsigned int bitpos = (int)mark;
    if (col == tcolor::BLACK)
        bitpos += 6;
    unsigned int bit = 1U << bitpos;
    this->bits |= bit;
}

void Marker::clear_marker(tmark mark, tcolor col)
{
    Marker::validate_marker(mark);
    unsigned int bitpos = (int)mark;
    if (col == tcolor::BLACK)
        bitpos += 6;
    unsigned int bit = 1U << bitpos;
    this->bits &= ~bit;
}

bool Marker::check_marker(tmark mark, tcolor col) const
{
    Marker::validate_marker(mark);
    unsigned int bitpos = (int)mark;
    if (col == tcolor::BLACK)
        bitpos += 6;
    unsigned int r = (this->bits >> bitpos) & 1U;
    return (bool) r;
}

bool Marker::check_other_marker(tcolor col) const
{
    col = other_color(col);
    unsigned int mask = 0b111111U;
    if (col == tcolor::BLACK)
        mask <<= 6;
    unsigned int r = this->bits & mask;
    return (bool) r;
}