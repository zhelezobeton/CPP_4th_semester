#include "Text.h"
#include <iostream>
#include <windows.h>

int main(int argc, char *argv[])
{
  SetConsoleOutputCP(1251); // русский язык
  SetConsoleCP(1251);
  if (argc == 3)
  {
    if (std::stoi(argv[2]) >= 1)
    {
      std::ifstream file(argv[1]);
      if (!file.is_open())
      {
        std::cerr << "ошибка открытия файла " << argv[1] << std::endl;
        return 1;
      }
      Text data(file, std::stoi(argv[2]));
      data.redact();
      data.print();
    }
    else
    {
      std::cerr << "некорректная длина строки!" << std::endl;
      return 3;
    }
  }
  else
  {
    std::cerr << "неверное количество аргументов" << std::endl;
    return 2;
  }
  return 0;
}
