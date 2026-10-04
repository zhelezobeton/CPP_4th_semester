#ifndef ONE_STRING_H
#define ONE_STRING_H

#include <string>
#include <vector>

class OneString
{
private:
  std::vector<std::string> words;

public:
  OneString(const std::string &ln);
  OneString(const std::vector<std::string> &ws);

  int length() const;
  std::string centered(int req_n) const;
  const std::vector<std::string> &get_words() const;
};

#endif