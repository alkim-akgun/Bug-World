#include "Player.hpp"

#include "utility.hpp"
#include "config.hpp"

Player::Player(const fs::path & p) : file(p), score(0)
{
  this->set_name(p.filename().generic_string());
}

Player::Player(const fs::path & file,
               const fs::path & dir) : Player(file)
{
  this->set_name_relative(dir);
}

void Player::set_name_relative(const fs::path & dir)
{
  this->name = dirfile_get_basename(this->file, dir/BUG_DIRECTORY);
}

void Player::add_score(int a)
{
  std::lock_guard<std::mutex> lock(m);
  this->score += a; 
}

