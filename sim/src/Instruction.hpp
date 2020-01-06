#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <sstream>
#include <variant> // C++17

#include "types.hpp"

class World;
class Bug;

#define PRNG_SEED 12345 // PRNG seed

class Instruction
{
    public:
    virtual ~Instruction(void) = default;
    virtual void parse(std::string) = 0;
    virtual void execute(Bug &, World &) = 0;

    protected:
    Instruction(void) = default;
    static bool pre_execute_check(Bug &);
    static bool EOL_check(std::istringstream &);
    
};

class I_sense : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate success, fail;
    tsensedir sensedir;
    tcondition cond;
};

class I_flip : public Instruction
{
    public:
    I_flip(void); // calls prng_init
    void parse(std::string);
    void execute(Bug &, World &);
    
    private:
    tstate success, fail;
    int p;
    static int prng_state;
    static void prng_init(void);
    static void prng_next_state(void);
    static int randomint(int); // PRNG 
};

class I_direction : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate success, fail;
    tdirection dir;
};


class I_turn : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate next;
    tleftright lr;
};

class I_pickup : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate success, fail;
};

class I_mark : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate next;
    tmark mark;
};

class I_unmark : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate next;
    tmark mark;
};

class I_move : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate success, fail;
};

class I_drop : public Instruction
{
    public:
    void parse(std::string);
    void execute(Bug &, World &);

    private:
    tstate next;
};

// I_generic defines I_*
typedef std::variant<I_sense, I_flip, I_direction, I_turn,
                       I_pickup, I_mark, I_unmark, I_move,
                       I_drop> I_generic;

#endif