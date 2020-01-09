#include "Tournament.hpp"

#include "config.hpp"

#include <boost/algorithm/string/predicate.hpp>

void Tournament::play()
{
  this->registerFiles();

  if (this->registry.maps.size() == 0)
  {
    throw std::runtime_error("No map files found");
  }

  size_t bs = this->registry.bugs.size();
  if (bs < 2)
  {
    throw std::runtime_error(std::to_string(bs)
                              + " bug" + (bs != 1 ? "s" : "")
                              + " found. Need at least 2");
  }

  this->preparePlayers();

  for (auto mapFile : this->registry.maps)
  {
    for (size_t i = 0; i < this->players.size()-1; i++)
    {
      for(size_t j = i+1; j < this->players.size(); j++)
      {
        this->matchPool.push_back(std::make_shared<Match>
          (mapFile.generic_string(), this->players[i],
           this->players[j], this->cycles));
        this->matchPool.back()->play();
      }
    }
  }
  
  this->joinThreads();
}

void Tournament::registerFiles()
{
  fs::path mappath(this->registry.directory / MAP_DIRECTORY);
  fs::path bugpath(this->registry.directory / BUG_DIRECTORY);

  if (!fs::is_directory(mappath))
  {
    throw std::runtime_error(mappath.generic_string() + " is not a directory");
  }

  if (!fs::is_directory(bugpath))
  {
    throw std::runtime_error(bugpath.generic_string() + " is not a directory");
  }

  fs::recursive_directory_iterator end;
  fs::recursive_directory_iterator it;

  for (it = fs::recursive_directory_iterator(mappath);
        it != end; ++it)
  {
    if (fs::is_regular_file(it->status())
        && boost::iequals(it->path().extension().generic_string(), MAP_EXTENSION))
    {
      this->registry.maps.push_back(it->path());
    }
  }

  for (it = fs::recursive_directory_iterator(bugpath);
        it != end; ++it)
  {
    if (fs::is_regular_file(it->status())
        && boost::iequals(it->path().extension().generic_string(), BUG_EXTENSION))
    {
      this->registry.bugs.push_back(it->path());
    }
  }
}


void Tournament::preparePlayers()
{
  for (auto bugfile : this->registry.bugs)
  {
    this->players.push_back(
      std::make_shared<Player>(bugfile, this->registry.directory));
  }
}


void Tournament::joinThreads()
{
  for (auto m : this->matchPool)
  {
    m->joinThreads();
  }
}
