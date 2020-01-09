#include "World.hpp"

#include <fstream>
#include <iostream>

#include "utility.hpp"

void World::load(std::string worldfile, std::string redbugfile,
                 std::string blackbugfile)
{
    // don't allow this method to be called more than once
    if (this->loaded)
    {
        std::string msg = "an instance of World was attempted to be "
                          "loaded a second time";
        throw std::runtime_error(msg);
    }
    else
        this->loaded = true;

    std::ifstream wstr(worldfile);
    if (!wstr.is_open())
    {
        std::string msg = "worldfile " + worldfile
                        + " cannot be opened";
        this->error(msg);
    }
    wstr >> this->width >> this->height;

    if (this->width <= 0 || this->height <= 0)
    {
        std::string msg = "width " + std::to_string(this->width)
                        + " or height " + std::to_string(this->height)
                        + " is not valid. Both must be positive.";
        this->error(msg);
    }

    int bugcount = 0;
    this->bugs.reserve(this->height * this->width);

    this->cells = std::vector<std::vector<Cell>>(this->height);

    for (int i = 0; i < this->height; i++)
    {
        this->cells[i].reserve(this->width);

        std::string line;
        wstr >> line;
        if (line.length() != (unsigned int)(this->width))
        {
            std::string msg = "invalid world file content (Line "
                            + std::to_string(i+1) + ")";
            this->error(msg);
        }

        for (int j = 0; j < this->width; j++)
        {
            char sym = line[j];
            switch (sym) // check cell validity
            {
                case '#': // obstructed
                case '.': // free cell
                case '+': // red home
                case '-': // black home
                    break;
                default:
                    if (sym < '1' || sym > '9') // not food either
                    {
                        std::string msg = std::string("invalid symbol '")
                                        + sym + "' for the cell at position ("
                                        + std::to_string(i) + ", "
                                        + std::to_string(j) + ").";
                        this->error(msg);
                    }
            }

            this->cells[i].emplace_back(Cell(sym));
            Cell & this_cell = this->cells[i].back(); // hold a reference

            // if home area, create a bug
            if (this_cell.is_red_home_area()
                || this_cell.is_black_home_area())
            {
                tcolor c;
                if (this_cell.is_red_home_area())
                    c = tcolor::RED;
                else if (this_cell.is_black_home_area())
                    c = tcolor::BLACK;
                else
                    unexpected_error("world.cpp: World::load");

                this->bugs.emplace_back(
                        Bug(bugcount++, c, (int)c, REST_NEEDED_PER_MOVE) );
                this->bugs.back().set_position(tposition(i, j));
                this_cell.set_occupant_ptr(&(this->bugs.back()));
            }
        }
    }
    std::string msg = "World initialized with "
                    + std::to_string(bugcount) + " bugs.\n";
    this->log(msg);

    // programs[tcolor] should give the program corresponding to that color
    // prog_id of each program is the same as the color they command
    this->programs[0] = Program((int)tcolor::RED, redbugfile, this);
    this->programs[1] = Program((int)tcolor::BLACK, blackbugfile, this);
}

void World::execute_cycle(int cycle)
{
    std::string msg = "Executing cycle " + std::to_string(cycle) + "\n";
    this->log(msg);
    for (Bug & b : this->bugs)
    {
        int bpid = b.get_progid();
        Program & p = this->programs[bpid];
        if (p.get_id() != bpid)
        {
            std::string msg = "unexpected program id mis-match";
            this->error(msg);
        }
        p.step(b);
    }
}

Cell & World::get_cell(tposition pos)
{
    return this->cells[pos.first][pos.second];
}

bool World::is_valid_position(tposition pos) const
{
    int x = pos.first;
    int y = pos.second;

    if(x < 0 || x > this->height
       || y < 0 || y > this->width)
        return false;
    return true;
}

int World::adjacent_other_bugs(tposition pos, tcolor col)
{
    int count = 0;
    col = other_color(col);
    for (int i = 0; i < 6; i++)
    {
        tdirection dir = static_cast<tdirection>(i);
        tposition check_pos = adjacent(pos, dir);
        Cell & check_cell = this->get_cell(check_pos);
        if (check_cell.is_occupied()
            && check_cell.get_occupant_ptr()->get_color() == col)
            count++;
    }
    return count;
}

void World::kill_at(tposition pos)
{
    Bug & b = *(this->get_cell(pos).get_occupant_ptr());
    b.kill();
    Cell & c = this->get_cell(pos);
    int f = c.get_food() + 3 + (b.get_food() ? 1 : 0);
    c.set_food(f);
    c.set_occupant_ptr(nullptr);
    std::string msg = "Bug at " + std::to_string(pos)
                        + " is killed\n";
    this->log(msg);
}

bool World::kill_if_surrounded(tposition pos) // return whether got killed
{
    Cell & c = this->get_cell(pos);
    if (!c.is_occupied())
        return false;
    tcolor col = c.get_occupant_ptr()->get_color();
    if (this->adjacent_other_bugs(pos, col) > 5)
    {
        this->kill_at(pos);
        return true;
    }
    return false;
}

void World::check_for_surrounded_bugs(tposition pos)
{
    for (int i = 0; i < 6; i++)
    {
        tdirection dir = static_cast<tdirection>(i);
        tposition check_pos = adjacent(pos, dir);
        this->kill_if_surrounded(check_pos);
    }
}

tcolor World::winner(void) const
{
    int rf = this->red_food();
    int bf = this->black_food();

    if (rf > bf)
        return tcolor::RED;
    else if (bf > rf)
        return tcolor::BLACK;
    else
        return tcolor::NONE; // DRAW
}

int World::red_food(void) const
{
    int sum = 0;
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            const Cell & c = this->cells[i][j];
            if (c.is_red_home_area())
                sum += c.get_food();
        }
    }

    return sum;
}

int World::black_food(void) const
{
    int sum = 0;
    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            const Cell & c = this->cells[i][j];
            if (c.is_black_home_area())
                sum += c.get_food();
        }
    }
    
    return sum;
}

void World::error(std::string msg)
{
    throw std::runtime_error(msg);
}

void World::log(std::string msg)
{
    if (!(this->keep_log)) return;

    static std::ofstream logstr("bug_world.log", std::ofstream::app);
    if (logstr.is_open())
    {;
        logstr << msg;
    }
    else
    {
        this->error("log file could not be opened");
    }
    
    
}