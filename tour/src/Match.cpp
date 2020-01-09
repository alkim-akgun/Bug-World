#include "Match.hpp"

Match::Match(const std::string & map, const std::shared_ptr<Player> p1,
             const std::shared_ptr<Player> p2, int cyc)
             : map(map), cycles(cyc)
{
  this->players[0] = p1;
  this->players[1] = p2;
  this->games[0].reset(new Game(map, p1->get_file(), p2->get_file(), false));
  this->games[1].reset(new Game(map, p2->get_file(), p1->get_file(), false));
}

void Match::play()
{
  this->threads[0] = std::thread(&Match::playGame, this, 0);
  this->threads[1] = std::thread(&Match::playGame, this, 1);
}

void Match::playGame(size_t id) // thread function
{
  this->games[id]->simulate(this->cycles, false, false, 0, "", true);
  score s = this->games[id]->get_score();
  this->results[id] = s;
  this->players[id]->add_score(s.first);
  this->players[1-id]->add_score(s.second);

  this->games[id].reset(nullptr);
}

void Match::joinThreads()
{
  this->threads[0].join();
  this->threads[1].join();
}
