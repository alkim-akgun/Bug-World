#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <vector>
#include <string>

#include "Instruction.hpp"

class World;
class Bug;

class Program
{
    private:
    int id;
    World* this_world_ptr;
    std::vector<I_generic> instr_list;

    public:
    Program(void) = default;
    Program(int, std::string, World*);
    inline int get_id(void) { return this->id; }
    void step(Bug &);
};

#endif