#include "Input.h"
#include "Library.h"

#include <array>
#include <limits>
#include <utility>

namespace
{
  void menu()
  {
    std::cout << "1 Добавить | 2 Список | 3 Выдать | 4 Вернуть\n"
              << "5 Удалить | 6 Сохранить | 7 Загрузить | 8 Меню | 0 Выход\n";
  }

  void addItem(Library& library)
  {
    const std::array types{"BOOK", "MAGAZINE", "DVD"};
    const std::array creators{"Автор: ", "Издательство: ", "Режиссёр: "};
    const std::array amounts{"Страниц: ", "Номер выпуска: ", "Минут: "};
    const auto kind = console::number<int>("Тип 1 BOOK, 2 MAGAZINE, 3 DVD: ", 1, 3) - 1;
    auto title = console::text("Название: ");
    const int year = console::number<int>("Год (1..2100): ", 1, 2100);
    auto creator = console::text(creators[kind]);
    const int amount = console::number<int>(amounts[kind], 1, 100000);
    library.add(MediaItem::create(types[kind], std::move(title), year, std::move(creator), amount));
    std::cout << "Добавлено издание " << types[kind] << '\n';
  }

  std::size_t readIndex()
  {
    return static_cast<std::size_t>(console::number<int>("Индекс: ", 0, std::numeric_limits<int>::max()));
  }
}

int main()
{
  Library library;
  std::cout << "Лабораторная 16. Библиотека, продвинутый вариант 3\n";
  menu();
  for (;;)
  {
    try
    {
      const int choice = console::number<int>("\nДействие: ", 0, 8);
      if (choice == 0)
      {
        break;
      }
      switch (choice)
      {
      case 1:
        addItem(library);
        break;
      case 2:
        library.print(std::cout); // Через базовый интерфейс выбираются детали каждого наследника.
        break;
      case 3:
      {
        auto& item = library.at(readIndex());
        item.issue(console::text("Читатель: "));
        std::cout << "Экземпляр выдан\n";
        break;
      }
      case 4:
        library.at(readIndex()).giveBack();
        std::cout << "Экземпляр возвращён\n";
        break;
      case 5:
        library.remove(readIndex());
        std::cout << "Издание удалено, индексы обновлены\n";
        break;
      case 6:
        library.save(console::text("Файл: "));
        std::cout << "Библиотека сохранена\n";
        break;
      case 7:
        library.load(console::text("Файл: "));
        std::cout << "Библиотека загружена\n";
        break;
      case 8:
        menu();
        break;
      }
    }
    catch (const std::exception& error)
    {
      if (!std::cin)
      {
        std::cout << "\nВвод завершён\n";
        break;
      }
      std::cout << "Ошибка: " << error.what() << '\n';
    }
  }
}
