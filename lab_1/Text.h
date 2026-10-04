#ifndef TEXT_H
#define TEXT_H

#include "OneString.h"
#include <fstream>
#include <vector>

class Text
{
private:
  std::vector<OneString> all_lines;
  std::vector<OneString> res_lines;
  int n;

public:
  Text(std::ifstream &file, int t_n);
  void redact();
  void print() const;
};

#endif