#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <vector>
#include <string>

#include "Instruction.hpp"

class Program
{
    private:
    std::string code;
    std::vector<I_generic> instr_list;
    
    public:
    Program(std::string);
    void process(void);
    void output(void);
    void error(std::string);
};

#endif