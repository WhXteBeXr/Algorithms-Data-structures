#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace console
{
  std::string line(std::string_view prompt);
  std::string text(std::string_view prompt);

  template <typename T>
  T number(std::string_view prompt, T minimum, T maximum)
  {
    for (;;)
    {
      try
      {
        std::istringstream input(line(prompt));
        T value{};
        if (!(input >> value))
        {
          throw std::invalid_argument("Ожидалось целое число допустимого типа");
        }
        input >> std::ws;
        if (!input.eof())
        {
          throw std::invalid_argument("После числа есть лишние символы");
        }
        if (value < minimum || value > maximum)
        {
          throw std::out_of_range("Число вне допустимого диапазона");
        }
        return value;
      }
      catch (const std::logic_error& error)
      {
        // Ошибка локального разбора не портит cin, следующая строка читается заново.
        std::cout << "Ошибка ввода: " << error.what() << '\n';
      }
    }
  }
}
