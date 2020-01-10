#include "utility.hpp"

std::string dirfile_get_basename(const fs::path & file,
                              const fs::path & directory)
{
  std::string child = file.generic_string();
  std::string base = directory.generic_string();
  if (base.back() != '/')
    base.push_back('/');
  if (child.length() <= base.length())
    return child; // invalid arguments. dont modify
  // find the last common character position
  size_t i = 0;
  for (; i < base.length(); i++)
  {
    if (child[i] != base[i])
    {
      i--;
      break;
    }
  }

  return std::string(child.begin()+i,
                      child.end()-file.extension().generic_string().length());
}