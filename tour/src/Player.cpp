#include "Player.hpp"

#include "config.hpp"

static std::string bugfile_relative_path(std::string, std::string);

Player::Player(const fs::path & p) : score(0)
{
  this->file = p.generic_string();
  this->set_name(p.filename().generic_string());
}

Player::Player(const fs::path & file,
               const fs::path & dir) : Player(file)
{
  this->set_name_relative(dir);
}

void Player::set_name_relative(const fs::path dir)
{
  this->name = bugfile_relative_path(
    this->file, (dir/BUG_DIRECTORY).generic_string() );
}

void Player::add_score(int a)
{
  std::lock_guard<std::mutex> lock(m);
  this->score += a; 
}

// base is a folder
std::string bugfile_relative_path(std::string child, std::string base)
{
  if (base.back() != '/')
    base.push_back('/');
  if (child.length() < base.length())
    return child;
  size_t i = 0;
  for (; i < base.length(); i++)
  {
    if (child[i] != base[i])
    {
      i--;
      break;
    }
  }

  std::string rel(child.begin()+i,
                  child.end()-std::string(BUG_EXTENSION).length());
  return rel;
}
