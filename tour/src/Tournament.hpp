#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include <string>
#include <vector>
#include <memory>

#include <experimental/filesystem>

#include "Player.hpp"
#include "Match.hpp"

namespace fs = std::experimental::filesystem;

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

  inline Tournament(std::string dir, int cyc) : cycles(cyc) {
    this->registry.directory = fs::path(dir); }
  void play();
  inline std::string get_directory() {
    return this->registry.directory.generic_string(); }
};

#endif
