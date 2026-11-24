#include "Input.h"

namespace console
{
  // Выводит подсказку и возвращает целую строку с пробелами. EOF передаёт в меню как исключение.
  std::string line(std::string_view prompt)
  {
    std::cout << prompt;
    std::string value;
    // Не смешиваем getline с чтением отдельных слов, поэтому пробелы внутри поля сохраняются.
    if (!std::getline(std::cin, value))
    {
      throw std::runtime_error("Ввод завершён");
    }
    return value;
  }

  // Повторяет запрос, пока строка не содержит хотя бы один непробельный символ.
  std::string text(std::string_view prompt)
  {
    for (;;)
    {
      auto value = line(prompt);
      // Проверка не обрезает строку, пользовательский текст возвращается целиком.
      if (value.find_first_not_of(" \t\r") != std::string::npos)
      {
        return value;
      }
      std::cout << "Ошибка ввода: строка не должна быть пустой\n";
    }
  }
}
