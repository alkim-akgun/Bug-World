#include "types.hpp"

#include <iostream>
#include <type_traits>


// re-direct the non-template qualifying signatures to a template function
// for tcondition, tdirection, tleftright and tsensedir only
template<typename T>
static std::istream & stream_to_t(std::istream & istr, T & tvar)
{
    std::string temp;
    istr >> temp;
    bool found = false;
    for (int i = 0; i < (int)T::Count; i++)
    {
        if (temp == std::to_string(static_cast<T>(i)))
        {
            tvar = static_cast<T>(i);
            found = true;
            break;
        }
    }
    if (!found)
    {
        std::string msg = "invalid type value " + temp
                        + ". Options:";
        for (int i = 0; i < (int)T::Count; i++)
        {
            msg += " " + std::to_string(static_cast<T>(i));
        }
        throw std::runtime_error(msg);
    }
    return istr;
}

// these operators cannot be templated
// because the linker won't prioritize templated definitions
// and give undefined reference at linking time 
std::istream & operator>>(std::istream & istr, tcondition & cond)
{
    return stream_to_t(istr, cond);
}
std::istream & operator>>(std::istream & istr, tdirection & dir)
{
    return stream_to_t(istr, dir);
}
std::istream & operator>>(std::istream & istr, tleftright & lr)
{
    return stream_to_t(istr, lr);
}
std::istream & operator>>(std::istream & istr, tsensedir & sd)
{
    return stream_to_t(istr, sd);
}

namespace std
{
    string to_string(tposition pos)
    {
        return "(" + to_string(pos.first) + ", "
                + to_string(pos.second) + ")";
    }
    string to_string(tcondition cond)
    {
        switch (cond)
        {
            case tcondition::FRIEND:
                return "friend";
            case tcondition::FOE:
                return "foe";
            case tcondition::FRIENDWITHFOOD:
                return "friendwithfood";
            case tcondition::FOOWITHFOOD:
                return "foowithfood";
            case tcondition::FOOD:
                return "food";
            case tcondition::ROCK:
                return "rock";
            case tcondition::HOME:
                return "home";
            case tcondition::FOEHOME:
                return "foehome";
            case tcondition::FOEMARKER:
                return "foemarker";
            default: //tcondition::MARKERX
                int mark = (int)cond;
                if (mark < 0 || mark > 5)
                {
                    std::string msg = "invalid tcondition "
                                    + std::to_string(mark);
                    throw std::runtime_error(msg);
                }
                return "marker" + to_string(mark);
        }
    }

    string to_string(tdirection dir)
    {
        switch (dir)
        {
            case tdirection::EAST:
                return "east";
            case tdirection::SOUTHEAST:
                return "southeast";
            case tdirection::SOUTHWEST:
                return "southwest";
            case tdirection::WEST:
                return "west";
            case tdirection::NORTHWEST:
                return "northwest";
            case tdirection::NORTHEAST:
                return "northeast";
            default:
                    std::string msg = "invalid tdirection "
                                    + std::to_string((int)dir);
                    throw std::runtime_error(msg);
        }
        return std::string();
    }

    string to_string(tleftright lr)
    {
        switch (lr)
        {
            case tleftright::LEFT:
                return "left";
            case tleftright::RIGHT:
                return "right";
            default:
                std::string msg = "invalid tleftright "
                                + std::to_string((int)lr);
                throw std::runtime_error(msg);
        }
    }

    string to_string(tsensedir sd)
    {
        switch (sd)
        {
            case tsensedir::HERE:
                return "here";
            case tsensedir::LEFTAHEAD:
                return "leftahead";
            case tsensedir::RIGHTAHEAD:
                return "rightahead";
            case tsensedir::AHEAD:
                return "ahead";
            default:
                std::string msg = "invalid tsensedir "
                                + std::to_string((int)sd);
                throw std::runtime_error(msg);
        }
    }

    string to_string(tcolor col)
    {
        if (col == tcolor::RED)
            return "Red";
        else if (col == tcolor::BLACK)
            return "Black";
        else // NONE falls here
        {
            std::string msg = "invalid tcolor "
                                + std::to_string((int)col);
            throw std::runtime_error(msg);
        }
        return "";
    }

}
