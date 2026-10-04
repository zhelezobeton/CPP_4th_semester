#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <windows.h>

// Класс-обёртка для списка слов одной строки
class WordContainer
{
private:
  std::vector<std::string> words_;

public:
  WordContainer() = default;

  // Конструктор: разбивает исходную строку на слова через istream_iterator
  explicit WordContainer(const std::string &rawLine)
  {
    std::istringstream stream(rawLine);
    std::istream_iterator<std::string> start(stream);
    std::istream_iterator<std::string> end;
    words_.assign(start, end);
  }

  // Обработка слов, длина которых превышает targetWidth (в т.ч. строк без пробелов)
  void splitOversizedWords(size_t targetWidth)
  {
    if (targetWidth == 0)
      return;

    std::vector<std::string> normalizedWords;
    for (const auto &word : words_)
    {
      if (word.length() <= targetWidth)
      {
        normalizedWords.push_back(word);
      }
      else
      {
        // Если слово длиннее лимита, разрезаем его на фрагменты
        for (size_t i = 0; i < word.length(); i += targetWidth)
        {
          normalizedWords.push_back(word.substr(i, targetWidth));
        }
      }
    }
    words_ = std::move(normalizedWords);
  }

  bool empty() const { return words_.empty(); }

  // Поддержка range-based for
  auto begin() const { return words_.begin(); }
  auto end() const { return words_.end(); }
};

// Класс-обёртка для форматированного текста
class TextFormatter
{
private:
  size_t targetWidth_;
  std::vector<std::string> formattedLines_;

  // Выравнивание по центру добавлением пробелов слева
  std::string centerAlign(const std::string &text) const
  {
    if (text.length() >= targetWidth_)
    {
      return text;
    }
    size_t totalSpaces = targetWidth_ - text.length();
    size_t leftPadding = totalSpaces / 2;
    return std::string(leftPadding, ' ') + text;
  }

public:
  explicit TextFormatter(size_t width) : targetWidth_(width) {}

  // Обработка одной строки файла: перенос слов и центрирование
  void processLine(const std::string &rawLine)
  {
    WordContainer container(rawLine);

    // Пустые строки сохраняем без изменений
    if (container.empty())
    {
      formattedLines_.emplace_back("");
      return;
    }

    // Если есть слова длиннее targetWidth, разбиваем их
    container.splitOversizedWords(targetWidth_);

    std::string currentLine;
    for (const auto &word : container)
    {
      if (currentLine.empty())
      {
        currentLine = word;
      }
      else if (currentLine.length() + 1 + word.length() <= targetWidth_)
      {
        currentLine += " " + word;
      }
      else
      {
        formattedLines_.push_back(centerAlign(currentLine));
        currentLine = word;
      }
    }

    if (!currentLine.empty())
    {
      formattedLines_.push_back(centerAlign(currentLine));
    }
  }

  // Вывод сформированных строк
  void print(std::ostream &out = std::cout) const
  {
    for (const auto &line : formattedLines_)
    {
      out << line << '\n';
    }
  }
};

int main(int argc, char *argv[])
{
  SetConsoleOutputCP(1251); // русский язык
  SetConsoleCP(1251);

  if (argc < 3)
  {
    std::cerr << "Использование: " << argv[0] << " <путь_к_файлу> <длина_строки>\n";
    return 1;
  }

  const std::string filename = argv[1];
  size_t lineWidth = 0;

  try
  {
    int parsed = std::stoi(argv[2]);
    if (parsed <= 0)
    {
      throw std::invalid_argument("Длина строки должна быть больше нуля.");
    }
    lineWidth = static_cast<size_t>(parsed);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Ошибка: неверная длина строки (" << e.what() << ")\n";
    return 1;
  }

  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Ошибка: не удалось открыть файл '" << filename << "'\n";
    return 1;
  }

  TextFormatter formatter(lineWidth);
  std::string line;

  // Читаем файл построчно через std::getline
  while (std::getline(file, line))
  {
    formatter.processLine(line);
  }

  file.close();

  // Выводим отформатированный результат
  formatter.print(std::cout);

  return 0;
}