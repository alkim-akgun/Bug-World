#include "Instruction.hpp"

#include "Labels.hpp"

void Instruction::label_not_found(std::string label_name)
{
    std::string msg = "label '" + label_name + "' is not found.";
    this->error(msg);
}

void Instruction::error(const std::string msg)
{
    throw std::runtime_error(msg);
}

// check if the 'else' keyword is received
bool Instruction::else_check(const std::string & else_str)
{
    if (else_str != "else")
    {
        std::string msg = "expected 'else', received '" + else_str + "'";
        this->error(msg);
        return false;
    }
    return true;
}

// check if the end of line is reached
bool Instruction::EOL_check(std::istringstream & liss)
{
    std::string temp;

    if (liss >> temp)
    {
        std::string msg = "expected end of line, received '" + temp + "'";
        this->error(msg);
        return false;
    }
    return true;
}


int I_default::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string word;
    liss >> word;
    if (word == "goto")
    {
        liss >> word;
        Labels::getInstance().add_goto(word, line_no);
    }
    else if (word.back() == ':') // label introduction
    {
        word.pop_back(); // remove : at the end
        Labels::getInstance().add_label(word, line_no);
    }
    else
    {
        std::string msg = "I_default.parse failed for the line '" + line + "'";
        this->error(msg);
    }
    
    return line_no;
}

std::string I_default::read(void)
{
    // first line goto instructions must be checked specifically
    // because there isn't a preceding line to trigger checking
    if (this->line_no == 1)
    {
        std::string label_name;
        int success_state = Labels::getInstance()
                                    .resolve_goto(this->line_no, label_name);

        if (label_name != "" // this instruction is a goto (not a label definition)
            && success_state == -1) // but the corresponding label is not found
        {
            this->label_not_found(label_name);
        }
    }

    return "";
}


// input: sense sensedir condition else on_fail_label
// output: sense sensedir success_state fail_state condition
int I_sense::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string else_str;
    std::string dummy; // skip 'sense'
    liss >> dummy >> this->sensedir >> this->cond >> else_str >> this->on_fail;
    this->else_check(else_str);
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_sense::read(void)
{
    std::string label_name;
    int success_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (success_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    int fail_state = Labels::getInstance().line_of(this->on_fail);
    if (fail_state == -1) // label not found
    {
        this->label_not_found(this->on_fail);
    }
    std::string output = "sense " + std::to_string(this->sensedir) + " "
                        + std::to_string(success_state) + " "
                        + std::to_string(fail_state) + " "
                        + std::to_string(this->cond);
    return output;
}


// input: flip p else on_fail_label
// output: flip p success_state fail_state
int I_flip::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string else_str;
    std::string dummy; // skip 'flip'
    liss >> dummy >> this->p >> else_str >> this->on_fail;
    this->else_check(else_str);
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_flip::read(void)
{
    std::string label_name;
    int success_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (success_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    int fail_state = Labels::getInstance().line_of(this->on_fail);
    if (fail_state == -1) // label not found
    {
        this->label_not_found(this->on_fail);
    }
    std::string output = "flip " + std::to_string(this->p) + " "
                        + std::to_string(success_state) + " "
                        + std::to_string(fail_state);
    return output;
}


// input: direction dir else on_fail_label
// output: direction dir success_state fail_state
int I_direction::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string else_str;
    std::string dummy; // skip 'direction'
    liss >> dummy >> this->dir >> else_str >> this->on_fail;
    this->else_check(else_str);
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_direction::read(void)
{
    std::string label_name;
    int success_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (success_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    int fail_state = Labels::getInstance().line_of(this->on_fail);
    if (fail_state == -1) // label not found
    {
        this->label_not_found(this->on_fail);
    }
    std::string output = "direction " + std::to_string(this->dir) + " "
                        + std::to_string(success_state) + " "
                        + std::to_string(fail_state);
    return output;
}


// input: turn dir
// output: turn dir next_state
int I_turn::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string dummy; // skip 'turn'
    liss >> dummy >> this->lr;
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_turn::read(void)
{
    std::string label_name;
    int next_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (next_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    std::string output = "turn " + std::to_string(this->lr) + " "
                        + std::to_string(next_state);
    return output;
}


// input: pickup else on_fail_label
// output: pickup success_state fail_state
int I_pickup::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string else_str;
    std::string dummy; // skip 'pickup'
    liss >> dummy >> else_str >> this->on_fail;
    this->else_check(else_str);
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_pickup::read(void)
{
    std::string label_name;
    int success_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (success_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    int fail_state = Labels::getInstance().line_of(this->on_fail);
    if (fail_state == -1) // label not found
    {
        this->label_not_found(this->on_fail);
    }
    std::string output = "pickup "
                        + std::to_string(success_state) + " "
                        + std::to_string(fail_state);
    return output;
}


// input: mark mark_m
// output: mark mark_m next_state
int I_mark::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string dummy; // skip 'mark'
    liss >> dummy >> this->mark;
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_mark::read(void)
{
    std::string label_name;
    int next_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (next_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    std::string output = "mark " + std::to_string(this->mark) + " "
                        + std::to_string(next_state);
    return output;
}


// input: unmark mark_m
// output: unmark mark_m next_state
int I_unmark::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string dummy; // skip 'unmark'
    liss >> dummy >> this->mark;
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_unmark::read(void)
{
    std::string label_name;
    int next_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (next_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    std::string output = "unmark " + std::to_string(this->mark) + " "
                        + std::to_string(next_state);
    return output;
}


// input: move else on_fail_label
// output: move success_state fail_state
int I_move::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string else_str;
    std::string dummy; // skip 'move'
    liss >> dummy >> else_str >> this->on_fail;
    this->else_check(else_str);
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_move::read(void)
{
    std::string label_name;
    int success_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (success_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    int fail_state = Labels::getInstance().line_of(this->on_fail);
    if (fail_state == -1) // label not found
    {
        this->label_not_found(this->on_fail);
    }
    std::string output = "move " + std::to_string(success_state) + " "
                        + std::to_string(fail_state);
    return output;
}


// input: drop
// output: drop next_state
int I_drop::parse(std::string line, int line_no)
{
    this->line_no = line_no;
    std::istringstream liss(line);
    std::string dummy; // skip 'drop'
    liss >> dummy;
    this->EOL_check(liss);
    return line_no+1;
}

std::string I_drop::read(void)
{
    std::string label_name;
    int next_state = Labels::getInstance()
                                .resolve_goto(this->line_no + 1, label_name);
    if (next_state == -1) // label not found
    {
        this->label_not_found(label_name);
    }
    std::string output = "drop " + std::to_string(next_state);
    return output;
}