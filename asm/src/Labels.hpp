#ifndef LABELS_HPP
#define LABELS_HPP

#include <map>

// Singleton Class
// must be accessible from every instruction
class Labels
{
    private:
    Labels() = default;

    public:
    static Labels& getInstance(void);
    Labels(Labels const &) = delete;
    void operator=(Labels const &) = delete;

    void add_label(std::string, int);
    int line_of(std::string);
    void add_goto(std::string, int);
    int resolve_goto(int, std::string &);

    std::map<std::string, int> label_list; // (label, line_no)
    std::map<int, std::string> goto_list;  // (line_no, label)
};

#endif