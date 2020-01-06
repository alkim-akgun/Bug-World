#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <sstream>
#include <variant> // C++17

#include "types.hpp"

class Instruction
{
    public:
    virtual ~Instruction(void) = default;
    virtual int parse(std::string, int) = 0;
    virtual std::string read(void) = 0;

    protected:
    Instruction(void) = default;
    int line_no;
    bool else_check(const std::string &);
    bool EOL_check(std::istringstream &);
    void label_not_found(std::string);
    static void error(const std::string);
    
};

// for labels and goto instructions
class I_default : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);
};

class I_sense : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);

    private:
    tsensedir sensedir;
    tcondition cond;
    std::string on_fail;
};

class I_flip : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);
    
    private:
    std::string on_fail;
    int p;
};

class I_direction : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);

    private:
    tdirection dir;
    std::string on_fail;
};


class I_turn : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);

    private:
    tleftright lr;
};

class I_pickup : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);

    private:
    std::string on_fail;
};

class I_mark : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);

    private:
    tmark mark;
};

class I_unmark : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);

    private:
    tmark mark;
};

class I_move : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);

    private:
    std::string on_fail;
};

class I_drop : public Instruction
{
    public:
    int parse(std::string, int);
    std::string read(void);
};

// I_generic defines I_*
typedef std::variant<I_sense, I_flip, I_direction, I_turn,
                       I_pickup, I_mark, I_unmark, I_move,
                       I_drop, I_default> I_generic;

#endif