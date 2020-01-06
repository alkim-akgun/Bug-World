#include <iostream>
#include <string>
#include <getopt.h>
#include <time.h>

#include "Game.hpp"

void printHelp(void)
{
    std::cout << "Usage: sim [OPTIONS] WORLDFILE REDBUGFILE BLACKBUGFILE\n"
                 "Simulator for Bug World.\n\n"
                 "Options:\n"
                 "  -n N, --cycles N             \t"
                 "Set the number of cycles to execute N\n"
                 "  -w, --slow                   \t"
                 "Wait after every cycle to ease observing the output. "
                 "Recommended to use with --cycles 250 --every 1 --log map\n"
                 "  -s, --stats                  \t"
                 "Display the statistics after the simulation\n"
                 "  -e E, --every E              \t"
                 "Print the state of the simulation every E cycles\n"
                 "  -r OPTION, --report OPTION\t"
                 "Set the format for reporting the state of the simulation\n"
                 "  -h, --help                   \t"
                 "Print this message\n\n"
                 "Options:\n"
                 "  map                          \t"
                 "Map view similar to the world file format\n"
                 "  debug                        \t"
                 "Complete report with one line for each cell"
              << std::endl;
}

int main(int argc, char* argv[])
{
    int cycles = 1000; // default number of cycles
    bool stats = false;
    bool slow = false;
    int every = 0;
    std::string report = "map"; // default

    const char* const short_options = "n:swe:l:h";
    const option long_options[] =
    {
        { "cycles", required_argument, nullptr, 'n'},
        { "stats",  no_argument,       nullptr, 's'},
        { "slow",   no_argument,       nullptr, 'w'},
        { "every",  required_argument, nullptr, 'e'},
        { "report", required_argument, nullptr, 'r'},
        { "help",   no_argument,       nullptr, 'h'},
        { 0,        0,                 0,        0 } // terminate
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
                case 's': // --stats
                    stats = true;
                    break;
                case 'w':
                    slow = true;
                    break;
                case 'e': // --every
                    every = std::stoi(optarg);
                    break;
                case 'r': // --report
                    report = std::string(optarg);
                    if (report != "debug" && report != "map")
                        goto help_exit_fail;
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
                help_exit_fail:
                    printHelp();
                    return EXIT_FAILURE;
                    break;
            }
        }
        
        if (argc - optind != 3)
            throw std::runtime_error("expected 3 file arguments, received "
                                     + std::to_string(argc - optind));

        std::string worldfile = argv[optind];
        std::string redbugfile = argv[optind+1];
        std::string blackbugfile = argv[optind+2];
 
        Game this_game(worldfile, redbugfile, blackbugfile);
        this_game.simulate(cycles, stats, slow, every, report, false); // not silent
    }
    catch(const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}