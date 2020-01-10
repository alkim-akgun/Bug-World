#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

#include "fs.hpp"

// removes extension and directory prefix from file path
// directory is the folder passed to tour as argument
// should only be used for files under this directory
std::string dirfile_get_basename(const fs::path &, const fs::path &);

#endif
