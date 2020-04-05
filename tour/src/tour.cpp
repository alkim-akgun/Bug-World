#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>
#include <algorithm>

#include "Tournament.hpp"
#include "config.hpp"
#include "utility.hpp"
#include "fs.hpp"


void printHelp();
void printSummary(const Tournament &);
void printLeaderboard(const Tournament &, bool save=false);
void writePlayer(std::ostream &, Player &);
void writeMatchResults(const Tournament &, const std::string);

std::string ARGV0;


void printHelp()
{
    std::cout << "Usage: tour [OPTIONS] DIR\n"
                 "Tournament simulator for Bug World.\n\n"
                 "Options:\n"
                 "  -n N, --cycles N             \t"
                 "Set the number of cycles to execute N\n"
                 "  -h, --help                   \t"
                 "Print this message\n\n"
              << std::endl;
}


int main(int argc, char* argv[])
{
    ARGV0 = std::string(argv[0]);

    int cycles = DEFAULT_CYCLES;
    const char* const short_options = "n:h";
    const option long_options[] =
    {
        { "cycles",    required_argument, nullptr, 'n'},
        { "help",      no_argument,       nullptr, 'h'},
        { 0,           0,                 0,        0 } // terminate
    };

    try
    {
        while (true)
        {
            const int c = getopt_long(argc, argv, short_options,
                                        long_options, nullptr);
            if (c == -1)
                break;

            switch (c)
            {
                case 'n': // --cycles
                    cycles = std::stoi(optarg);
                    break;
                case 'h': // --help
                    printHelp();
                    return EXIT_SUCCESS;
                    break;
                case ':': // option missing argument
                case '?': // invalid option
                    return EXIT_FAILURE;
                    break; // getopt prints an error
                default:
                    printHelp();
                    return EXIT_FAILURE;
                    break;
            }
        }
        
        if (argc - optind != 1)
            throw std::runtime_error("expected an argument for the directory");

        std::string directory(argv[optind]);


        Tournament tour(directory);
        tour.play(cycles);

        printSummary(tour);
        std::cout << '\n';
        std::cout.width(32);
        std::cout  << "LEADERBOARD\n";
        printLeaderboard(tour, true);
        writeMatchResults(tour, directory);
    }
    catch(const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


void printSummary(const Tournament & tour)
{
    std::cout << tour.players.size() << " bugs participated:\n";
    for (auto p_ptr : tour.players)
    {
        std::cout << '\t' << p_ptr->get_name() << '\n';
    }
    std::cout << "\n" << tour.get_registry().maps.size()
                << " maps played:\n";
    for (auto m : tour.get_registry().maps)
    {
        std::cout << '\t' << dirfile_get_basename(m, 
                        tour.get_registry().directory / MAP_DIRECTORY) + '\n';
    }
}


void printLeaderboard(const Tournament & tour, bool save)
{
    std::ofstream lbofs;
    if (save)
        lbofs.open(LEADERBOARD_FILE);

    std::vector<std::shared_ptr<Player>> leaderboard(tour.players.begin(),
                                                    tour.players.end());
    // sort in descending score order
    std::sort(leaderboard.begin(), leaderboard.end(),
              [](auto p1, auto p2) {return p1->get_score() >= p2->get_score();} );
    for (auto p_ptr : leaderboard)
    {
        writePlayer(std::cout, *p_ptr);
        if (save)
            writePlayer(lbofs, *p_ptr);
    }
    if (save)
        lbofs.close();
}


void writePlayer(std::ostream & ofs, Player & p)
{
    ofs.width(32);
    ofs << p.get_name();
    ofs.width(16);
    ofs << std::right << p.get_score() << '\n';
}


void writeMatchResults(const Tournament & tour, const std::string dir)
{
    int width1 = 32, width2 = 24;
    std::ofstream mrofs(MATCHRESULTS_FILE);
    for (const auto & m_ptr : tour.matchPool)
    {
        std::string map_name = dirfile_get_basename(fs::path(m_ptr->get_map()),
                                                    fs::path(dir)/MAP_DIRECTORY);
        mrofs << std::left << "Map: " << map_name << '\n';

        mrofs.width(width1);
        mrofs << "";
        mrofs.width(width2);
        mrofs << "Game 1 (R\\B)";
        mrofs.width(width2);
        mrofs << "Game 2 (B\\R)" << '\n';

        mrofs.width(width1);
        mrofs << m_ptr->get_players()[0]->get_name();
        mrofs.width(width2);
        mrofs << m_ptr->get_results()[0].first;
        mrofs.width(width2);
        mrofs << m_ptr->get_results()[1].second << "\n";

        mrofs.width(width1);
        mrofs << m_ptr->get_players()[1]->get_name();
        mrofs.width(width2);
        mrofs << m_ptr->get_results()[0].second;
        mrofs.width(width2);
        mrofs << m_ptr->get_results()[1].first << "\n\n";
    }
}
