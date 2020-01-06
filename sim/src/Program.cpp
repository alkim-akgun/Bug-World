#include "Program.hpp"

#include <fstream>
#include <string.h>
#include <iostream>
#include "World.hpp"
#include "Bug.hpp"

Program::Program(int id, std::string filename, World* this_world_ptr)
                : id(id), this_world_ptr(this_world_ptr)
{
    std::ifstream ifs(filename);
    std::string line;
    while(std::getline(ifs, line))
    {
        // detect if a comment starts
        size_t f = line.find(';');
        if (f != std::string::npos)
        {
            // erase the rest of the word
            line.erase(f);
        }
        // if the whole line was a comment
        if (line == "") continue; // move onto the next line

        // check the first word to determine the instruction
        // parse the instruction and save
        std::istringstream liss(line);
        std::string word;

        while (liss >> word)
        {
            if (word == "sense")
            {
                I_sense instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;
            }

            else if (word == "move")
            {
                I_move instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;            
            }

            else if (word == "pickup")
            {
                I_pickup instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;              
            }

            else if (word == "flip")
            {
                I_flip instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;
            }

            else if (word == "turn")
            {
                I_turn instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;             
            }

            else if (word == "drop")
            {
                I_drop instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;                
            }

            else if (word == "mark")
            {
                I_mark instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr); 
                break;               
            }

            else if (word == "unmark")
            {
                I_unmark instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);  
                break;              
            }

            else if (word == "direction")
            {
                I_direction instr;
                instr.parse(line);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);  
                break;              
            }

            else
            {
                const std::string msg = "parsing failed for the line:\n" + line;
                throw std::runtime_error(msg);
            }
        }
    }

}

void Program::step(Bug & b)
{
    //std::string msg = "Executing instruction: ";
    //this->this_world_ptr->log(msg);

    auto lambdaExecuteVisitor = [&b, this] (auto && instr) mutable
                                    { instr.execute(b, *(this->this_world_ptr)); };
    tstate s = b.get_state();
    if (s < 0 || s >= (int)this->instr_list.size())
    {
        std::string msg = "invalid next state " + std::to_string(s)
                        + " for the program controlling "
                        + std::to_string((int)b.get_color());
        throw std::runtime_error(msg);
    }
    auto instr = this->instr_list[s];
    std::visit(lambdaExecuteVisitor, instr);
    // instr will set the new state
}