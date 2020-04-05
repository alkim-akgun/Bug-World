#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include <string>
#include <vector>
#include <memory>

#include "Player.hpp"
#include "Match.hpp"
#include "fs.hpp"

struct Registry
{
  fs::path directory;
  std::vector<fs::path> maps;
  std::vector<fs::path> bugs;
};

class Tournament
{
  private:
  Registry registry;
  int cycles;
  void registerFiles();
  void preparePlayers();
  void joinThreads();


  public:
  std::vector<std::shared_ptr<Player>> players;
  std::vector<std::shared_ptr<Match>> matchPool;

  inline Tournament(std::string dir){ this->registry.directory = fs::path(dir); }
  void play(int);
  inline Registry get_registry() const { return this->registry; }
  inline Registry & get_registry() { return this->registry; }
  inline std::string get_directory() {
    return this->registry.directory.generic_string(); }
};

#endif
