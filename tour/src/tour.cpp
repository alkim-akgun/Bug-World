#include <iostream>
#include <string>
#include <getopt.h>
#include <vector>
#include <algorithm>

#include "Tournament.hpp"

#define DEFAULT_CYCLES 1000000

void printLeaderboard(const Tournament &);

void printHelp(void)
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

      std::string dir(argv[optind]);

      Tournament tour(dir, cycles);
      tour.play();
      printLeaderboard(tour);
  }
  catch(const std::exception& e)
  {
      std::cerr << argv[0] << ": " << e.what() << '\n';
      return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

void printLeaderboard(const Tournament & tour)
{
  std::vector<std::shared_ptr<Player>> lb(tour.players.begin(),
                                          tour.players.end()); 
  std::sort(lb.begin(), lb.end(),
              [](auto p1, auto p2) {return p1->get_score() >= p2->get_score();} );
  std::cout << "\tLEADER BOARD\n\n";
  for (auto p : lb)
  {
    std::cout << p->get_name() << "\t\t" << p->get_score() << '\n';
  }
}
