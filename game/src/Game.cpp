#include "Game.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include "types.hpp"
#include "Bug.hpp"
#include "Cell.hpp"

Game::Game(std::string worldfile, std::string redbugfile,
            std::string blackbugfile)
{
    this_world.load(worldfile, redbugfile, blackbugfile);
}

void Game::simulate(int cycles, bool stats, bool slow, int every,
              std::string report, bool silent)
{
    if (cycles < 0)
    {
        std::string msg = "number of cycles to execute cannot "
                          "be a negative integer";
        throw std::invalid_argument(msg);
    }

    if (every < 0)
    {
        std::string msg = "every argument cannot be a negative integer";
        throw std::invalid_argument(msg);
    }
    
    if (!silent)
        print_state(report, this_world, 0);
    int i;
    for (i = 1; i <= cycles; i++)
    {
        this_world.execute_cycle(i);
        if (every > 0 && i % every == 0)
            print_state(report, this_world, i);
        // sleep
        if (slow)
            std::this_thread::sleep_for(
                std::chrono::milliseconds(SLOW_MODE_SLEEP_MILLISECONDS));
    }
    if (!silent)
        print_state(report, this_world, -1);
    if (stats)
    {
        std::cout << "\n";
        print_stats(this_world);
    }
}

void Game::print_stats(World & W)
{
    std::cout << "Red bugs collected " << W.red_food()
              << " units of food.\n"
              << "Black bugs collected " << W.black_food()
              << " units of food.\n";
    if (W.winner() == tcolor::NONE)
        std::cout << "It's a draw.";
    else
        std::cout << "Winner is " << std::to_string(W.winner()) << ".";
    std::cout << std::endl;
}

void Game::print_state(std::string report, World & W, int cycle)
{
    if (cycle != 0)
        std::cout << "\n";

    if (cycle == 0)
        std::cout << "INITIAL STATE...\n";
    else if (cycle == -1)
        std::cout << "FINAL STATE...\n";
    else
        std::cout << "AFTER CYCLE " << cycle << "...\n";

    if (report == "map")
        print_map_state(W);
    else if (report == "debug")
        print_debug_state(W);
    else
    {
        std::string msg = "unknown report argument " + report + " received";
        throw std::invalid_argument(msg);
    }
}

void Game::print_map_state(World & W)
{
    for (int i = 0; i < W.get_height(); i++)
    {
        // gives the hexagonal shape - no need
        //if (i % 2 == 1) std::cout << " ";

        for (int j = 0; j < W.get_width(); j++)
        {
            Cell & c = W.get_cell(tposition(i, j));
            // cell description order:
            // obstructed > bug > food > home > free 
            if (c.get_obstructed()) // obstructed
                std::cout << "#";
            else if (c.get_occupant_ptr() != nullptr) // bug
            {
                Bug & b = *c.get_occupant_ptr();
                if (b.get_color() == tcolor::RED)
                    std::cout << "R";
                else if (b.get_color() == tcolor::BLACK)
                    std::cout << "B";
            }
            else
            {
                if (c.get_food() > 9) // food
                    std::cout << ">"; // indicating more than 9 food
                else if (c.get_food() > 0) // food
                    std::cout << c.get_food();
                else
                {
                    if (c.is_red_home_area()) // home
                        std::cout << "+";
                    else if (c.is_black_home_area()) // home
                        std::cout << "-";
                    else
                        std::cout << "."; // free
                }
            }
            if (j != W.get_width()-1)
                std::cout << " ";
        }
        std::cout << "\n";
    }
}

void Game::print_debug_header(bool repeat)
{
    std::cout << "========== cell ========== ======= bug ======";
    if (repeat)
        std::cout << " (continued)";
    std::cout << "\n"
                 "        b b                                  \n"
                 "        a i                    cbd           \n"
                 "pos pos s t  red    black      oii           \n"
                 " x   y  e s  marks  marks  id  ltr state rest\n"
                 "=== === = == ====== ====== === === ===== ====\n";
}

void Game::print_debug_state(World & W)
{
    if (W.get_height() >= 1000
        || W.get_width() >= 1000)
    {
        std::string msg = "debug format does not support "
                          " height or width more than 999";
        throw std::runtime_error(msg);
    }

    int line = 0;
    for (int i = 0; i < W.get_height(); i++)
    {
        for (int j = 0; j < W.get_width(); j++)
        {
            if (line == 0)
                print_debug_header(false);
            else if (debug_HEADER_EVERY != 0 && (line % debug_HEADER_EVERY) == 0)
                print_debug_header(true);
            line++;

            Cell & c = W.get_cell(tposition(i, j));
            // cell
                      // x coordinate
            std::cout << std::setfill('0') << std::setw(3) << i << " "
                      // y coordinate
                      << std::setfill('0') << std::setw(3) << j << " ";
            // base
            if (c.is_red_home_area())
                std::cout << "r";
            else if (c.is_black_home_area())
                std::cout << "b";
            else
                std::cout << "-";
            std::cout << " ";
            // food
            if (c.get_food() > 99)
                std::cout << ">>";
            else
                std::cout << std::setfill('0') << std::setw(2)
                          << c.get_food();
            std::cout << " ";
            // red marks
            for (tmark m = 5; (int)m >= 0; m--)
                std::cout << (c.marks.check_marker(m, tcolor::RED) ?
                                            std::to_string(m) : "_");
            std::cout << " ";
            // black marks
            for (tmark m = 5; (int)m >= 0; m--)
                std::cout << (c.marks.check_marker(m, tcolor::BLACK) ?
                                        std::to_string(m) : "_");
            std::cout << " ";
            // bug
            Bug* b_ptr = c.get_occupant_ptr();
            if (b_ptr == nullptr) // no bug
                std::cout << std::string(18, ' '); // pad line
            else
            {
                Bug & b = *b_ptr;
                // bug id
                if (b.get_id() > 999)
                    std::cout << ">>>";
                else
                    std::cout << std::setfill('0') << std::setw(3)
                              << b.get_id();
                // bug color
                if (b.get_color() == tcolor::RED)
                    std::cout << " r";
                else
                    std::cout << " b";
                // bug food
                if (b.get_food())
                    std::cout << "X";
                else
                    std::cout << "_";
                // bug dir
                std::cout << (int)b.get_direction() << " ";
                // bug state
                if (b.get_state() > 99999)
                    std::cout << ">>>>> ";
                else
                    std::cout << std::setfill('0') << std::setw(5)
                              << b.get_state() << " ";
                // bug remaining rest
                std::cout << std::setfill('0') << std::setw(4)
                          << b.get_remaining_rest();
            }
            if (i < W.get_height()-1)
                std::cout << "\n";
            else // if last line, also flush buffer
                std::cout << std::endl;
        }
    }
}