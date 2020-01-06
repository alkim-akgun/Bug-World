#include "Program.hpp"

#include <string.h>
#include <iostream>

Program::Program(std::string code) : code(code) { }

void Program::error(std::string msg)
{
    throw std::runtime_error(msg);
}

void Program::process(void)
{   
    int line_no = 0;
    std::istringstream iss(this->code);
    std::string line;
    while (std::getline(iss, line))
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
            // I_default
            if (word.back() == ':' || word == "goto")
            {
                I_default instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;
            }

            else if (word == "sense")
            {
                I_sense instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;
            }

            else if (word == "move")
            {
                I_move instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;            
            }

            else if (word == "pickup")
            {
                I_pickup instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;              
            }

            else if (word == "flip")
            {
                I_flip instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;
            }

            else if (word == "turn")
            {
                I_turn instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;             
            }

            else if (word == "drop")
            {
                I_drop instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);
                break;                
            }

            else if (word == "mark")
            {
                I_mark instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr); 
                break;               
            }

            else if (word == "unmark")
            {
                I_unmark instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);  
                break;              
            }

            else if (word == "direction")
            {
                I_direction instr;
                line_no = instr.parse(line, line_no);
                I_generic g_instr = instr;
                this->instr_list.emplace_back(g_instr);  
                break;              
            }

            else
            {
                const std::string msg = "parsing failed for the line:\n" + line;
                this->error(msg);
            }
        }
    }
}

void Program::output()
{
    std::string out;
    static auto lambdaReadVisitor = [&out](auto && instr) mutable
                                    { out = instr.read(); };

    for (int i = 0; i < (int)this->instr_list.size(); i++)
    {
        auto instr = this->instr_list[i];
        std::visit(lambdaReadVisitor, instr);
        if (out != "")
            std::cout << out << std::endl;
    }
}