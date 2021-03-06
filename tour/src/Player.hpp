#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <experimental/filesystem>
#include <mutex>
#include <string>

#include "fs.hpp"
#include "score.hpp"

class Player
{
  private:
  fs::path file; // absolute path
  std::string name; // path relative to directory
  int score;
  std::mutex m;

  public:
  Player() = default;
  Player(const fs::path &); // if base directory is not known
  Player(const fs::path &, const fs::path &);
  inline std::string get_file(void) const { return this->file; }
  inline std::string get_name(void) const { return this->name; }
  inline void set_name(std::string name) { this->name = name; }
  void set_name_relative(const fs::path &);
  inline int get_score(void) const { return this->score; }
  void add_score(int);
};

#endif
