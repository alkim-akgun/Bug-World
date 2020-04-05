#include "Match.hpp"

//#include <exception>
#include <iostream>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>


#include "fs.hpp"
#include "utility.hpp"

#define SIM_NAME "sim"
#define READ_BUF_SIZE 64

Match::Match(const std::string & map, const std::shared_ptr<Player> p1,
             const std::shared_ptr<Player> p2, int cyc)
             : map(map), cycles(cyc)
{
  this->players[0] = p1;
  this->players[1] = p2;
}

std::mutex Match::sm;

void Match::play()
{
  this->threads[0] = std::thread(&Match::playGame, this, 0);
  this->threads[1] = std::thread(&Match::playGame, this, 1);
}

void Match::playGame(size_t id) // thread function
{
  // id determines whether first (0) or second (1) match

  try
  {
    // hello C, my old friend
    
    int r;

    // Simulate
    if (PIPE_BUF < READ_BUF_SIZE)
    {
        throw std::runtime_error("PIPE_BUF is smaller than READ_BUF_SIZE");
    }

    int fd[2];
    r = pipe2(fd, O_DIRECT);
    if(r == -1)
    {
        perror("pipe2 failed");
        throw std::runtime_error("pipe2 failed");
    }
                            // TO DO: std::system()
    int pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        throw std::runtime_error("fork failed");
    }

    if (pid == 0) // child
    {
        std::cerr << "Child calling execl\n";
        extern std::string ARGV0;
        const std::string SIM_PATH(ARGV0.substr(0, ARGV0.find_last_of('/')+1) + SIM_NAME);
        std::cerr << "SIM_PATH: " + SIM_PATH + "\n";
        //          cmd       args
        r = execl(SIM_PATH.c_str(), SIM_PATH.c_str(),
                                    "--worker",
                                    std::to_string(fd[1]).c_str(),
                                    "--cycles",
                                    std::to_string(this->cycles).c_str(),
                                    this->map.c_str(),
                                    this->players[id]->get_file().c_str(),
                                    this->players[1-id]->get_file().c_str(),
                                    (char*) NULL);
        if (r == -1)
        {
            perror("execl failed");
            exit(EXIT_FAILURE);
        }

        return;
    }


    /*
    r = system(cmd.c_str());
    if (r == -1)
    {
        perror("system failed");
        throw std::runtime_error("system failed");
    }
    */
    int wstatus;
    r = waitpid(pid, &wstatus, 0); // there are more than one children
    if (r == -1)
    {
        perror("waitpid failed");
        throw std::runtime_error("waitpid failed");
    }
    std::cerr << "child (pid: " + std::to_string(pid) + ") terminated\n";
    if(!WIFEXITED(wstatus))
    {
        throw std::runtime_error("child did not terminate normally");
    }

    // Read
    int score1 = -1, score2 = -1;
    const int buf_size = READ_BUF_SIZE;
    char buf[buf_size];
    ssize_t s = read(fd[0], buf, buf_size-1);
    printf("buf: %s\n", buf);
    if (s == -1)
    {
        perror("read failed");
      throw std::runtime_error("read failed");
    }
    else if (s == 0)
    {
      throw std::runtime_error("0 bytes read from sim");
    }
    close(fd[0]);

    // buf should be "score1 score2\0"

    int p = 0;
    while(buf[p] != ' ' && buf[p] != '\0' && p < s)
    {
      p++;
    }

    // buf[p] should be ' ' (space) here
    buf[p] = '\0'; // divide into 2 C-strings

    if (buf[0] == '\0')
    {
      throw std::runtime_error("invalid score1 received from sim");
    }
    score1 = std::atoi(buf);

    if (buf[p+1] == '\0' || p+1 >= s)
    {
      throw std::runtime_error("invalid score2 received from sim");
    }
    score2 = std::atoi(buf+p+1);


    this->players[id]->add_score(score1);
    this->players[1-id]->add_score(score2);
    std::lock_guard<std::mutex> lock(m);
    this->results[id] = std::make_pair(score1, score2);
  }
  catch (const std::exception & e)
  {
    std::lock_guard<std::mutex> lock(this->sm);
    std::cerr << "Thread (id: " << std::this_thread::get_id() << "): "
              << std::string(e.what()) << std::endl;
  }
  
}

void Match::joinThreads()
{
  this->threads[0].join();
  this->threads[1].join();
}
