#include "Text.h"
#include <iostream>

Text::Text(std::ifstream &file, int t_n) : n(t_n)
{
  std::string line;
  while (std::getline(file, line))
  {
    all_lines.push_back(OneString(line));
  }
}

void Text::redact()
{
  std::vector<std::string> current;
  for (const auto &l : all_lines)
  {
    if (!current.empty())
    {
      res_lines.push_back(OneString(current));
      current.clear();
    }
    const std::vector<std::string> &buffer = l.get_words();
    for (const auto &b : buffer)
    {
      current.push_back(b);
      if (OneString(current).length() > n)
      {
        current.pop_back();
        if (!current.empty())
        {
          res_lines.push_back(OneString(current));
          current.clear();
          current.push_back(b);
        }
        else
        {
          int pos = 0;
          while ((pos + n) < b.size())
          {
            res_lines.push_back(OneString({b.substr(pos, n)}));
            pos += n;
          }
          current.push_back(b.substr(pos, n));
        }
      }
    }
  }
  if (!current.empty())
  {
    res_lines.push_back(OneString(current));
  }
}
void Text::print() const
{
  for (const auto &r : res_lines)
  {
    if (!r.get_words().empty())
    {
      std::cout << r.centered(n) << std::endl;
    }
  }
}