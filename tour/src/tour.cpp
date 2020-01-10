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
void writeLeaderboard(const Tournament &);

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
      writeLeaderboard(tour);
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
    std::cout << "Bugs participated:\n";
    for (auto p_ptr : tour.players)
    {
        std::cout << '\t' << p_ptr->get_name() << '\n';
    }
    std::cout << "\nMaps played on:\n";
    for (auto m : tour.get_registry().maps)
    {
        std::cout << '\t' << dirfile_get_basename(m, 
                        tour.get_registry().directory / MAP_DIRECTORY) + '\n';
    }
}

void writeLeaderboard(const Tournament & tour)
{
    
    std::ofstream ofs(LEADERBOARD_FILE);
    std::vector<std::shared_ptr<Player>> leaderboard(tour.players.begin(),
                                                    tour.players.end()); 
    std::sort(leaderboard.begin(), leaderboard.end(),
              [](auto p1, auto p2) {return p1->get_score() >= p2->get_score();} );
    for (auto p_ptr : leaderboard)
    {
        ofs << p_ptr->get_name() << "\t\t" << p_ptr->get_score() << '\n';
    }
    ofs.close();
}
