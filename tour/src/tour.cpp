#include <iostream>
#include <string>
#include <getopt.h>
#include <time.h>
#include <vector>
#include <thread>
#include <atomic>

#include "Game.hpp"

struct GameConfig{
  std::string worldFile;
  int cycles = 1000;
  bool stats = false;
  bool slow = false;
  int every = 0;
  std::string report = "map";
} globalConfig;

struct Player{
  std::string gameFile;
  int id;
  int score=0;
};

struct Match{
  std::shared_ptr<Game> game;
  std::thread* runningThread;
  Player *redBug, *blackBug;
};

std::vector<Player> players;

void simulateTournament(){
  std::vector<std::shared_ptr<Match>> matchPool;

  for (size_t i=0; i<players.size(); i++){
    for (size_t j=0; j<players.size(); j++){
      if (i==j) {
        continue;
      }
      auto redBug = &players[i];
      auto blackBug = &players[j];
      auto thisGame = std::make_shared<Game>(globalConfig.worldFile, redBug->gameFile, blackBug->gameFile);
      auto thisThread = new std::thread([thisGame]() {thisGame->simulate(globalConfig.cycles, globalConfig.stats, globalConfig.slow, globalConfig.every, globalConfig.report, true); });
      auto match = std::shared_ptr<Match>(new Match{thisGame, thisThread, redBug, blackBug});
      matchPool.push_back(match);
    }
  }
  for (auto& iter:matchPool){
    iter->runningThread->join();
    // Must delete thread before Game, segfaults otherwise
    delete iter->runningThread;
    auto res = iter->game->get_score();
    iter->redBug->score+=res.first;
    iter->blackBug->score+=res.second;
  }
  for (auto iter:players) {
    std::cout<<"Score for player #"<<iter.id<<": "<<iter.score<<'\n';
  }
}

void printHelp(void)
{
    std::cout << "Usage: tour [OPTIONS] WORLDFILE [PLAYERFILES]\n"
                 "Tournament simulator for Bug World.\n\n"
                 "Options:\n"
                 "  -n N, --cycles N             \t"
                 "Set the number of cycles to execute N\n"
                 "  -w, --slow                   \t"
                 "Wait after every cycle to ease observing the output. "
                 "Recommended to use with --cycles 250 --every 1 --log map\n"
                 "  -h, --help                   \t"
                 "Print this message\n\n"
              << std::endl;
}

int main(int argc, char* argv[])
{
    const char* const short_options = "n:swe:l:h";
    const option long_options[] =
    {
        { "cycles", required_argument, nullptr, 'n'},
        { "slow",   no_argument,       nullptr, 'w'},
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
                    globalConfig.cycles = std::stoi(optarg);
                    break;
                    break;
                case 'w':
                    globalConfig.slow = true;
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
        
        if (argc - optind < 3)
            throw std::runtime_error("expected >=3 file arguments, received "
                                     + std::to_string(argc - optind));

        globalConfig.worldFile = argv[optind];

        for(int iter = optind+1, id=0; iter<argc; iter++,id++){
          players.push_back(Player{argv[iter], id});
        }

        simulateTournament();
    }
    catch(const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
