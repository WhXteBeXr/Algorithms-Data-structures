#include "Input.h"

namespace console
{
  std::string line(std::string_view prompt)
  {
    std::cout << prompt;
    std::string value;
    if (!std::getline(std::cin, value))
    {
      throw std::runtime_error("Ввод завершён");
    }
    return value;
  }

  std::string text(std::string_view prompt)
  {
    for (;;)
    {
      auto value = line(prompt);
      if (value.find_first_not_of(" \t\r") != std::string::npos)
      {
        return value;
      }
      std::cout << "Ошибка ввода: строка не должна быть пустой\n";
    }
  }
}
