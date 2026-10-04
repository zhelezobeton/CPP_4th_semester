#include "OneString.h"
#include <iterator>
#include <sstream>

OneString::OneString(const std::string &ln)
{
  std::istringstream iss(ln);
  for (std::istream_iterator<std::string> it(iss), end; it != end; ++it)
  {
    words.push_back(*it);
  }
}
OneString::OneString(const std::vector<std::string> &ws) : words(ws) {}

int OneString::length() const // длина строки с прбелами между словами
{
  int total = 0;
  if (words.empty())
  {
    return total;
  }
  for (const auto &w : words)
  {
    total += (w.size() + 1);
  }
  return total - 1;
}

std::string OneString::centered(int req_n) const // центрирование строки по нужной длине
{
  std::string s;
  if (words.empty())
  {
    return "";
  }
  if (req_n > length())
  {
    s += std::string(((req_n - length()) / 2), ' ');
  }
  for (const auto &w : words)
  {
    s += w + ' ';
  }
  if (s.back() == ' ')
  {
    s.pop_back();
  }
  return s;
}

const std::vector<std::string> &OneString::get_words() const { return words; }