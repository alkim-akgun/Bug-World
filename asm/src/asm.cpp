#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Program.hpp"

int main(int argc, char* argv[])
{
    try
    {
        if (argc > 2)
        {
            std::string msg = "expected at most 1 argument, "
                            + std::to_string(argc-1) + " provided";
            throw std::invalid_argument(msg);
        }

        else if(argc == 2 && (strcmp(argv[1], "-h") == 0 
                              || strcmp(argv[1], "--help") == 0 ))
        {
            std::cout << "Usage: asm [OPTION] [FILE]\n"
                         "Assembler for Bug World.\n\n"
                         "If FILE is not provided or is -, "
                         "read from standard input.\n\n"
                         "Options:\n  -h, --help\t"
                         "Print this message."
                      << std::endl;
        }
        else
        {
            std::string code;

            if (argc == 1 || strcmp(argv[1], "-") == 0)
            {
                std::string line;
                while (std::getline(std::cin, line))
                {
                    code += line + '\n';
                }
            }
            else
            {
                std::string fname = std::string(argv[1]);
                std::ifstream ifstr(fname);
                if (!ifstr)
                {
                    std::string msg = "reading from file " + fname + " failed";
                    throw std::runtime_error(msg);
                }
                std::stringstream buff;
                buff << ifstr.rdbuf();
                code = std::move(buff.str());
                ifstr.close();
            }
            Program prog(code);
            prog.process();
            prog.output();
        }

        return EXIT_SUCCESS;
    }
    catch(const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << '\n';
        return EXIT_FAILURE;
    } 
}
