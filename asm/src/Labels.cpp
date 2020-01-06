#include "Labels.hpp"

Labels& Labels::getInstance(void)
{
    static Labels single;
    return single;
}


void Labels::add_label(std::string label, int line_no)
{
    this->label_list[label] = line_no;
}

int Labels::line_of(std::string label)
{
    auto f = this->label_list.find(label);
    if (f == this->label_list.end())
    {
        return -1;
    }
    return f->second;
}
void Labels::add_goto(std::string label, int line_no)
{
    this->goto_list[line_no] = label;
}

int Labels::resolve_goto(int line_no, std::string & label_name)
{
    // over-writes label_name to fetch it outside

    auto f = this->goto_list.find(line_no);

    // if 'goto' is not found in this line
    if (f == this->goto_list.end())
    {
        label_name = ""; // indicate that there is no goto
        // return the same line number to continue from
        return line_no; 
    }

    // over-write label_name for access from outside
    label_name = f->second;
    // fetch the state corresponding to the label
    int next_state = this->line_of(label_name);
    
    return next_state; // returns -1 if label is not found
}