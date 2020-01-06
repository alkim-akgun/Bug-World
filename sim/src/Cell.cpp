#include "Cell.hpp"

#include <string>

#include "types.hpp"

Cell::Cell(char sym)
{
    switch (sym)
    {
        case '#':
            this->obstructed = true;
            break;
        case '.':
            break;
        case '+':
            this->home_red_bug = true;
            break;
        case '-':
            this->home_black_bug = true;
            break;
        default:
            if (sym < '1' || '9' < sym)
            {
                std::string msg = std::string("Invalid cell symbol '")
                                + sym + "'.";
                throw std::runtime_error(msg);
            }
            else
                this->food = sym - '0';
    }
}

bool Cell::is_home_to_color(tcolor c) const
{
    if (c == tcolor::RED)
    {
        return this->is_red_home_area();
    }
    else if (c == tcolor::BLACK)
    {
        return this->is_black_home_area();
    }
    else
    {
        std::string msg = "invalid tcolor " + std::to_string((int)c);
        throw std::runtime_error(msg);
    } 
}