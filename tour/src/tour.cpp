#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>
#include <algorithm>

#include "Tournament.hpp"
#include "config.hpp"
#include "utility.hpp"

void printHelp(void);
void printSummary(const Tournament &);
void printLeaderboard(const Tournament &, bool save=false);
void writePlayer(std::ostream &, Player &);

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

      Tournament tour(directory, cycles);
      tour.play();

      printSummary(tour);
      std::cout.width(48);
      std::cout << "\n" << std::string(20, ' ') << "LEADERBOARD\n";
      printLeaderboard(tour, true);
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
    ofs << std::left << p.get_name();
    ofs.width(16);
    ofs << std::right << p.get_score() << '\n';
}
