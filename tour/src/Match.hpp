#ifndef MATCH_HPP
#define MATCH_HPP

#include <array>
#include <memory>
#include <thread>

#include "score.hpp"
#include "Player.hpp"
#include "Game.hpp"

typedef std::pair<score, score> matchResult;

// a match is 2 Games on 1 map with 2 Players switching order
class Match
{
  private:
  std::string map;
  std::array<std::shared_ptr<Player>, 2> players;
  std::array<std::unique_ptr<Game>, 2> games; // store games in the heap
  int cycles;
  std::array<std::thread, 2> threads;
  std::array<score, 2> results;
  void playGame(size_t); // thread function

  public:
  Match(const std::string &, const std::shared_ptr<Player>,
                             const std::shared_ptr<Player>, int);
  // Hard-copy not implemented. Avoid shallow copy
  Match(const Match &) = delete;
  Match & operator=(const Match &) = delete;
  void play();
  void joinThreads();
  inline std::string get_map(void) const { return this->map; }
  inline std::array<std::shared_ptr<Player>, 2> get_players() {
                                        return this->players; }
  inline std::array<score, 2> get_results(void) const { return this->results; }
};

#endif
